#include "./Animator.h"
#include "./AnimatorManager.h"

bool Animator::HasFinished(void) const {
	return state != ANIMATOR_RUNNING;
}

void Animator::Finish(bool isForced) {
	if (!HasFinished()) {
		state = isForced ? ANIMATOR_STOPPED : ANIMATOR_FINISHED;
		NotifyStopped();
	}
}

void Animator::Stop(void)
{
	Finish(true);
}

void Animator::NotifyStarted(void) {
	AnimatorManager::GetSingleton().MarkAsRunning(this);
	if (onStart)
		(onStart)(this);
}

void Animator::NotifyStopped(void) {
	AnimatorManager::GetSingleton().MarkAsSuspended(this);

	if (onFinish)
		(onFinish)(this);
}

void Animator::NotifyAction(const Animation& anim) {
	if (onAction)
		(onAction)(this, anim);
}

void Animator::TimeShift(timestamp_t offset)
{
	lastTime += offset;
}

/*template <typename Tfunc> */void Animator::SetOnFinish(const OnFinish& f) {
	onFinish = f;
}

/*template <typename Tfunc> */void Animator::SetOnStart(const OnStart& f) {
	onStart = f;
}

/*template <typename Tfunc> */void Animator::SetOnAction(const OnAction& f) {
	onAction = f;
}

Animator::Animator(void)
{
	AnimatorManager::GetSingleton().Register(this);
}

Animator::~Animator(void)
{
	AnimatorManager::GetSingleton().Cancel(this);
}
