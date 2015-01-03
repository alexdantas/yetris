#ifndef ANIMATIONFIRE_H_DEFINED
#define ANIMATIONFIRE_H_DEFINED

#include <Engine/Graphics/Animation.hpp>
#include <Engine/Helpers/Array2D.hpp>
#include <Engine/Helpers/Timer.hpp>

#include <vector>

// This intensity determines how each cell will get drawn.
// Bigger intensity means brighter colors and "emptier" characters.
#define INTENSITY_MIN 0
#define INTENSITY_MAX 100

/// Returns #x percent of the whole intensity set of values.
#define INTENSITY_PERCENT(x) \
	(x * (100/(INTENSITY_MAX - INTENSITY_MIN)))

/// A single particle inside the whole fire
/// (cell within the Window matrix).
struct ParticleFire
{
	int intensity;

	ParticleFire(int intensity=0):
		intensity(intensity)
	{ };
};

/// Shows a nice fire animation on the screen.
class AnimationFire: public Animation
{
public:
	AnimationFire(Window* window);
	virtual ~AnimationFire();

	void load();
	void update();
	void draw();

private:
	/// All particles.
	Array2D<ParticleFire>* particle;
	Array2D<int>* coolingMap;
	Timer timer;
};

#endif //ANIMATIONFIRE_H_DEFINED

