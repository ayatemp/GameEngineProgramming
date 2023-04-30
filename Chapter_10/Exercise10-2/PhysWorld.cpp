// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include <SDL/SDL.h>

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{
    mBoxes_x = mBoxes;
    mBoxes_y = mBoxes;
    mBoxes_z = mBoxes;
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;
	// Initialize closestT to infinity, so first
	// intersection will always update closestT
	float closestT = Math::Infinity;
	Vector3 norm;
	// Test against all boxes
	for (auto box : mBoxes)
	{
		float t;
		// Does the segment intersect with the box?
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			// Is this closer than previous intersection?
			if (t < closestT)
			{
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f)
{
	// Naive implementation O(n^2)
	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// Don't need to test vs itself and any previous i values
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* a = mBoxes[i];
			BoxComponent* b = mBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				// Call supplied function to handle intersection
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	// min.xによってソートする
	std::sort(mBoxes.begin(), mBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.x <
				b->GetWorldBox().mMin.x;
	});

	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// box[i]のmax.xを取得
		BoxComponent* a = mBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* b = mBoxes[j];
			// もしbox[j]のmin.xが，box[i]のmax.x境界を超えていたら
			// box[i]と交差する可能性があるボックスは，他に存在しない
    
			if (b->GetWorldBox().mMin.x > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
    
    // 課題１０−２--------------------------------------------------------------------///////
    //最小のx.y.zでソートする
    std::sort(mBoxes_x.begin(),mBoxes_x.end(),[](BoxComponent* a,BoxComponent* b)
              { return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x; } );
    std::sort(mBoxes_y.begin(),mBoxes_y.end(),[](BoxComponent* a,BoxComponent* b)
              { return a->GetWorldBox().mMin.y < b->GetWorldBox().mMin.y; } );
    std::sort(mBoxes_z.begin(),mBoxes_z.end(),[](BoxComponent* a,BoxComponent* b)
              { return a->GetWorldBox().mMin.z < b->GetWorldBox().mMin.z; } );
    
    //x軸
    for(size_t i = 0; i < mBoxes_x.size(); i++)
    {
        BoxComponent* a = mBoxes_x[i];
        float max = a->GetWorldBox().mMax.x;
        for(size_t j = i+1; j < mBoxes_x.size(); j++)
        {
            BoxComponent* b = mBoxes_x[j];
            
            if(b->GetWorldBox().mMin.x > max)
            {
                break;
            }
            //ここでx軸だけでみたら軸が重なっているので，連想配列に入れる
            test_x[b] = a;
        }
    }
    //y軸
    for(size_t i = 0; i < mBoxes_y.size(); i++)
    {
        BoxComponent* a = mBoxes_y[i];
        float max = a->GetWorldBox().mMax.y;
        for(size_t j = i+1; j < mBoxes_y.size(); j++)
        {
            BoxComponent* b = mBoxes_y[j];
            
            if(b->GetWorldBox().mMin.y > max)
            {
                break;
            }
            //ここでy軸だけでみたら軸が重なっているので，連想配列に入れる
            test_y[b] = a;
        }
    }
    //z軸
    for(size_t i = 0; i < mBoxes_z.size(); i++)
    {
        BoxComponent* a = mBoxes_z[i];
        float max = a->GetWorldBox().mMax.z;
        for(size_t j = i+1; j < mBoxes_z.size(); j++)
        {
            BoxComponent* b = mBoxes_z[j];
            
            if(b->GetWorldBox().mMin.z > max)
            {
                break;
            }
            //ここでz軸だけでみたら軸が重なっているので，連想配列に入れる
            test_z[b] = a;
        }
    }

    //全ての軸で重なり合うコンポーネントを探す,
    //重なっているものがいたら代入された関数を使用する
    for(size_t i = 0; i < mBoxes.size(); i++)
    {
        BoxComponent* a = mBoxes[i];
        if(test_x[a] == test_y[a] && test_x[a] == test_z[a] && test_y[a] == test_z[a])
        {
            BoxComponent* b = test_x[a];
            f(a->GetOwner(),b->GetOwner());
        }
    }
    //----------------------------------------------------------------------------------/////
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
	if (iter != mBoxes.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mBoxes.end() - 1);
		mBoxes.pop_back();
	}
}
