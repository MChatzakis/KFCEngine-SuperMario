#ifndef _FRAMERANGEANIMATION_H_
#define _FRAMERANGEANIMATION_H_

#include "./MovingAnimation.h"

class FrameRangeAnimation : public MovingAnimation {
protected:
	unsigned start = 0, end = 0;
public:
	using Me = FrameRangeAnimation;
	unsigned GetStartFrame(void) const;
	Me& SetStartFrame(unsigned v);
	unsigned GetEndFrame(void) const;
	Me& SetEndFrame(unsigned v);
	Animation* Clone(void) const override;
	FrameRangeAnimation(const std::string& _id,unsigned s, unsigned e,unsigned r, int dx, int dy, int d);

};

unsigned
FrameRangeAnimation::GetStartFrame(void) const { return start; }

FrameRangeAnimation::Me&
FrameRangeAnimation::SetStartFrame(unsigned v) { start = v; return *this; }

unsigned
FrameRangeAnimation::GetEndFrame(void) const { return end; }

FrameRangeAnimation::Me&
FrameRangeAnimation::SetEndFrame(unsigned v) { end = v; return *this; }

Animation*
FrameRangeAnimation::Clone(void) const //override
{
	return new FrameRangeAnimation(
		id, start, end, GetReps(), GetDx(), GetDy(), GetDelay()
	);
}

FrameRangeAnimation::FrameRangeAnimation(
	const std::string& _id,
	unsigned s, unsigned e,
	unsigned r, int dx, int dy, int d
) : start(s), end(e), MovingAnimation(id, r, dx, dy, d) {}

#endif _FRAMERANGEANIMATION_H_