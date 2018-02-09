#include "BaseDemo.h"

class FrameAnimationTest : BaseDemo
{
public:
	CREATE_FUNC(FrameAnimationTest);

	static cocos2d::Scene* createScene()
	{
		auto scene = cocos2d::Scene::create();
		auto layer = FrameAnimationTest::create();

		scene->addChild(layer);
		return scene;
	}

private:
	dragonBones::CCArmatureDisplay* _armatureDisplay;

protected:
	virtual void _onStart()
	{
		const auto factory = dragonBones::CCFactory::getFactory();
		factory->loadDragonBonesData("FrameAnimation/FramesAnimation_ske.json");
		factory->loadTextureAtlasData("FrameAnimation/FramesAnimation_tex.json");

		_armatureDisplay = factory->buildArmatureDisplay("boy-fall");
		_armatureDisplay->setPosition(0.0f, 0.0f);
		addChild(_armatureDisplay);

		// Enabled cocos2d Event
		_armatureDisplay->getEventDispatcher()->setEnabled(true);
		// Add animation event listener.
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::START, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN_COMPLETE, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT_COMPLETE, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&FrameAnimationTest::_animationEventHandler, this, std::placeholders::_1));
		_armatureDisplay->getAnimation()->play("boy-fall");
		//
		const auto listener = cocos2d::EventListenerMouse::create();
		listener->onMouseDown = CC_CALLBACK_1(FrameAnimationTest::_mouseDownHandler, this);
		listener->onMouseUp = CC_CALLBACK_1(FrameAnimationTest::_mouseUpHandler, this);
		listener->onMouseMove = CC_CALLBACK_1(FrameAnimationTest::_mouseMovedHandler, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		createText("Touch to control animation play progress.");
	}

private:
	void _mouseDownHandler(cocos2d::EventMouse* event)
	{
		const auto progress = std::min(std::max((event->getCursorX() - getPosition().x + 300.0f) / 600.0f, 0.0f), 1.0f);
		_armatureDisplay->getAnimation()->gotoAndStopByProgress("boy-fall", progress);
	}

	void _mouseUpHandler(cocos2d::EventMouse* event)
	{
		_armatureDisplay->getAnimation()->play();
	}

	void _mouseMovedHandler(cocos2d::EventMouse* event)
	{
		if (event->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT)
		{
			return;
		}

		const auto progress = std::min(std::max((event->getCursorX() - getPosition().x + 300.0f) / 600.0f, 0.0f), 1.0f);
		const auto animationState = _armatureDisplay->getAnimation()->getState("boy-fall");
		if (animationState)
		{
			animationState->setCurrentTime(animationState->getTotalTime() * progress);
		}
	}

	void _animationEventHandler(cocos2d::EventCustom* event) const
	{
		const auto eventObject = (dragonBones::EventObject*)event->getUserData();
		cocos2d::log("%s %s %s", eventObject->animationState->name.c_str(), eventObject->type.c_str(), eventObject->name.c_str());
	}
};