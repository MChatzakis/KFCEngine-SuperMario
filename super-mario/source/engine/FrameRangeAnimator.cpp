#include "./FrameRangeAnimator.h"

unsigned
FrameRangeAnimator::GetCurrFrame(void) const { return currFrame; }

unsigned
FrameRangeAnimator::GetCurrRep(void) const { return currRep; }

void
FrameRangeAnimator::Start(FrameRangeAnimation* a, timestamp_t t) {
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
	currFrame = anim->GetStartFrame();
	currRep = 0;
	NotifyStarted();
	NotifyAction(*anim);
}

void
FrameRangeAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		if (currFrame == anim->GetEndFrame()) {
			assert(anim->IsForever() || currRep < anim->GetReps());
			currFrame = anim->GetStartFrame(); // flip to start
		}
		else
			++currFrame;
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (currFrame == anim->GetEndFrame())
			if (!anim->IsForever() && ++currRep == anim->GetReps()) {
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
	}
}

/*void FrameRange_Action(Sprite* sprite, Animator* animator, const FrameRangeAnimation& anim) {
	auto* frameRangeAnimator = (FrameRangeAnimator*)animator;
	if (frameRangeAnimator->GetCurrFrame() != anim.GetStartFrame() ||
		frameRangeAnimator->GetCurrRep())
		sprite->Move(anim.GetDx(), anim.GetDy());
	sprite->SetFrame(frameRangeAnimator->GetCurrFrame());
}*/

/*animator->SetOnAction(
	[sprite](Animator* animator, const Animation& anim) {
		FrameRange_Action(sprite, animator, (const FrameRangeAnimation&)anim);
	}
);*/
