#pragma once
#include <cstdint>

class Component
{
public:
	// コンストラクタ
	// updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);
	
	// デストラクタ
	virtual ~Component();

	// このコンポーネントをデルタタイムで更新する
	virtual void Update(float deltaTime);

	// このコンポーネント用のProcess Input
	virtual void ProcessInput(const uint8_t* keyState){}

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// 所有アクター
	class Actor* mOwner;

	// コンポーネントの更新順序
	int mUpdateOrder;
};