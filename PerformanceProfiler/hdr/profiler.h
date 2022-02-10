//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

#define PROFILING_ENABLED 0

struct ProfileResult {
	std::string Name;
	long long Start, End;
	uint32_t ThreadID;
};

struct InstrumentationSession {
	std::string Name;
};

class Instrumentor {
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
public:
	Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

	void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
		m_OutputStream.open(filepath);
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void EndSession() {
		m_OutputStream << "]}";
		m_OutputStream.flush();
		m_OutputStream.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void WriteProfile(const ProfileResult& result) {
		if (m_ProfileCount > 2) {
			m_OutputStream << ',';
		}
		m_ProfileCount += 1;

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream
			<< "{\"cat\":\"function\",\"dur\":"
			<< (result.End - result.Start)
			<< ",\"name\":\""
			<< name << "\",\"ph\":\"X\",\"pid\":0,\"tid\":"
			<< result.ThreadID
			<< ",\"ts\":"
			<< result.Start
			<< "}";
	}

	static Instrumentor& Get() {
		static Instrumentor instance;
		return instance;
	}
};

class InstrumentationTimer {
	const char* m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	bool m_Stopped;
public:
	InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false) {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer() {
		if (!m_Stopped)
			Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

		m_Stopped = true;
	}
};



#if PROFILING_ENABLED
#define PROFILE InstrumentationTimer timer##__LINE__(__FUNCSIG__)
#define START_PROFILE Instrumentor::Get().BeginSession("Profile")
#define END_PROFILE Instrumentor::Get().EndSession()
#else
#define PROFILE
#define START_PROFILE
#define END_PROFILE
#endif

