
#include "pch.h"

// output operator
std::ostream& operator<<(std::ostream& stream, sf::Vector2f vector) {
	stream << "(" << vector.x << ", " << vector.y << ")";
	return stream;
}



namespace sf {

	// division operator
	Vector2f operator/(sf::Vector2f a, sf::Vector2f b) {
		a.x /= b.x;
		a.y /= b.y;
		return a;
	}

	// multiplication operator
	Vector2f operator*(sf::Vector2f a, sf::Vector2f b) {
		a.x *= b.x;
		a.y *= b.y;
		return a;
	}

	// multiplication assignment operator
	Vector2f operator*=(sf::Vector2f a, sf::Vector2f b) {
		a = a * b;
		return a;
	}

	Vector2f operator+=(sf::Vector2f a, sf::Vector2f b) {
		a = a + b;
		return a;
	}


	bool isSpriteInWindow(const sf::Sprite& sprite, const sf::RenderWindow& window) {
		sf::Vector2f spriteSize = (sf::Vector2f)sprite.getTexture()->getSize();
		sf::Vector2f spritePos = sprite.getPosition() + spriteSize;
		sf::Vector2f windowSize = (sf::Vector2f)window.getSize() + spriteSize;

		return (spritePos.x > 0.0f) && (spritePos.x < windowSize.x)
			&& (spritePos.y > 0.0f) && (spritePos.y < windowSize.y);
	}

	bool isColliding(const sf::Sprite& a, const sf::Sprite& b) {
		sf::Vector2f thisPos = a.getPosition();
		sf::Vector2f thisSize = (
			sf::Vector2f
			)(
				a.getTexture()
				->getSize()
				);

		sf::Vector2f targetPos = b.getPosition() - thisSize;
		sf::Vector2f targetSize = (sf::Vector2f)(b.getTexture()->getSize()) + thisSize;

		return (thisPos.x > targetPos.x) && (thisPos.x < targetPos.x + targetSize.x)
			&& (thisPos.y > targetPos.y) && (thisPos.y < targetPos.y + targetSize.y);
	}

	bool isViewObstructed(const sf::Sprite& a, const sf::Sprite& b, const std::vector<StaticEntity>& platforms) {

		sf::Vector2f aPos = a.getPosition();
		sf::Vector2f aSize = sf::Vector2f(a.getTexture()->getSize());

		sf::Vector2f bPos = b.getPosition();
		sf::Vector2f bSize = sf::Vector2f(b.getTexture()->getSize());


		// expand target by player size
		bPos -= aSize / 2.0f;
		bSize += aSize;

		// allow player to see over the platform a bit
		bPos.y += 5;
		bSize.y -= 5;

		// simulate a ray between the two sprites
		const sf::Vector2f& ray_origin = aPos + (aSize / 2.0f);
		const sf::Vector2f& ray_dir = bPos - aPos;

		for (const StaticEntity& platform : platforms) {
			const sf::Vector2f& pPos = platform.getPosition();
			const sf::Vector2f& pSize = (sf::Vector2f)platform.getTexture()->getSize();

			// check where in the ray a collision occurred
			sf::Vector2f t_near = (pPos - ray_origin) / ray_dir;
			sf::Vector2f t_far = (pPos + bSize - ray_origin) / ray_dir;

			// if any values are not a number then exit early
			if (std::isnan(t_far.y) || std::isnan(t_far.x) ||
				std::isnan(t_near.y) || std::isnan(t_near.x))
				continue;

			// make sure the near time is smaller than the far time
			if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
			if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

			// if a collision has not occurred then exit early
			if (t_near.x > t_far.y || t_near.y > t_far.x) continue;

			// get collision point
			float contact_time = std::max(t_near.x, t_near.y);
			float t_hit_far = std::min(t_far.x, t_far.y);

			// if the collision is behind or infront of the ray then exit early
			if (contact_time < 0.0f || contact_time >= 1.0f) continue;
			if (t_hit_far < 0) continue;

			return 1;
		}
		return 0;
	}

}