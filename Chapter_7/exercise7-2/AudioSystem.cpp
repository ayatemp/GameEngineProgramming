// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
	,mSystem(nullptr)
	,mLowLevelSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
	// エラーログの設定（初期化）
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // エラーログだけ受け取る
		FMOD_DEBUG_MODE_TTY // stdoutに出力
	);

	//　FMOD systemの作成（インスタンス）
    FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	// FMOD systemの初期化
	result = mSystem->initialize(
		512, // 最大同次発音数
		FMOD_STUDIO_INIT_NORMAL, // デフォルトの設定
		FMOD_INIT_NORMAL, // デフォルトの設定
		nullptr // 通常はnull
	);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	// LowLevelシステムポインタを取得して保存
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	// Load the master banks (strings first)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	// Unload all banks
	UnloadAllBanks();
	// Shutdown FMOD system
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name)
{
	// 多重読み込みの防止
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	// バンクのロード
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(), // File name of bank
		FMOD_STUDIO_LOAD_BANK_NORMAL, // Normal loading
		&bank // Save pointer to bank
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		// Add bank to map
		mBanks.emplace(name, bank);
		// Load all non-streaming sample data
		bank->loadSampleData();
		// Get the number of events in this bank
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			// Get list of event descriptions in this bank
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				// Get the path of this event (like event:/Explosion2D)
				e->getPath(eventName, maxPathLength, nullptr);
				// Add to event map
				mEvents.emplace(eventName, e);
			}
		}
		// Get the number of buses in this bank
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// Get list of buses in this bank
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// Get the path of this bus (like bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// Add to buses map
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	// Ignore if not loaded
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// First we need to remove all events from this bank
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		// Get event descriptions for this bank
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// Get list of events
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// Get the path of this event
			e->getPath(eventName, 512, nullptr);
			// Remove this event
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}
	// Get the number of buses in this bank
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		// Get list of buses in this bank
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			// Get the path of this bus (like bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			// Remove this bus
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	// Unload sample data and bank
	bank->unloadSampleData();
	bank->unload();
	// Remove from banks map
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		// Unload the sample data, then the bank itself
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	// No banks means no events
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
    //イベントの存在を確認
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		// イベントインスタンスを作成
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			// Start the event instance
			event->start();
			// Get the next id, and add to map
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
	// ストップしたイベントインスタンスを探す
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		// イベントの状態を取得
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			// イベントを開放してidを終了リストに追加
			e->release();
			done.emplace_back(iter.first);
		}
	}
	
	// 終了したイベントインスタンスを連想配列から削除
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	// FMOD更新
	mSystem->update();
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// 我々の座標系（＋xが前方，＋yが右，＋zが上）
		// からFMODの座標系（＋zが前方，＋xが右，＋yが上）に置換
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}


//課題７−２
void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	// ベクトルを得るためのビュー行列の逆行列を計算する
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// Set position, forward, up
	listener.position = VecToFMOD(invView.GetTranslation());
	// In the inverted view, third row is forward
	listener.forward = VecToFMOD(invView.GetZAxis());
	// In the inverted view, second row is up
	listener.up = VecToFMOD(invView.GetYAxis());
	// Set velocity to zero (fix if using Doppler effect)
	listener.velocity = {0.0f, 0.0f, 0.0f};
	// Send to FMOD
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string & name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string & name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}
