#include "./MovingAnimator.h"


auto
MovingAnimator::GetAnim(void) const -> const MovingAnimation&
{
	return *anim;
}
void
MovingAnimator::Start(MovingAnimation* a, timestamp_t t) {
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
	currRep = 0; //?
	NotifyStarted();
	//NotifyAction(*anim); //frame range animator has notify action here
}

void
MovingAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}

void Sprite_MoveAction(Sprite* sprite, const MovingAnimation& anim) {
	sprite->Move(anim.GetDx(), anim.GetDy());
}

