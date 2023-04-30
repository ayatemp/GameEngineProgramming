//
//  ArrowTarget.cpp
//  Game-mac
//
//  Created by 加来彩人 on 2021/12/08.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//


#include "ArrowTarget.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "ArrowComponent.h"

ArrowTarget::ArrowTarget(Game* game)
    :Actor(game)
{
    SetScale(1.0f);
    SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
    mc->SetMesh(mesh);
    
    mBox = new BoxComponent(this);
    mBox->SetObjectBox(mesh->GetBox());
    
    new ArrowComponent(this);
}

