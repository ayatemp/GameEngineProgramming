// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "LevelLoader.h"
#include <fstream>
#include <vector>
#include <SDL/SDL.h>
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "BallActor.h"
#include "FollowActor.h"
#include "PlaneActor.h"
#include "TargetActor.h"
#include "Component.h"
#include "AudioComponent.h"
#include "BallMove.h"
#include "BoxComponent.h"
#include "CameraComponent.h"
#include "FollowCamera.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "MirrorCamera.h"
#include "PointLightComponent.h"
#include "TargetComponent.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <iostream>

const int LevelVersion = 1;

// Declare map of actors to spawn functions
std::unordered_map<std::string, ActorFunc> LevelLoader::sActorFactoryMap
{
	{ "Actor", &Actor::Create<Actor> },
	{ "BallActor", &Actor::Create<BallActor> },
	{ "FollowActor", &Actor::Create<FollowActor> },
	{ "PlaneActor", &Actor::Create<PlaneActor> },
	{ "TargetActor", &Actor::Create<TargetActor> },
};

std::unordered_map<std::string, std::pair<int, ComponentFunc>> LevelLoader::sComponentFactoryMap
{
	{ "AudioComponent", { Component::TAudioComponent, &Component::Create<AudioComponent>} },
	{ "BallMove", { Component::TBallMove, &Component::Create<BallMove> } },
	{ "BoxComponent", { Component::TBoxComponent, &Component::Create<BoxComponent> } },
	{ "CameraComponent", { Component::TCameraComponent, &Component::Create<CameraComponent> } },
	{ "FollowCamera", { Component::TFollowCamera, &Component::Create<FollowCamera> } },
	{ "MeshComponent", { Component::TMeshComponent, &Component::Create<MeshComponent> } },
	{ "MoveComponent", { Component::TMoveComponent, &Component::Create<MoveComponent> } },
	{ "SkeletalMeshComponent", { Component::TSkeletalMeshComponent, &Component::Create<SkeletalMeshComponent> } },
	{ "SpriteComponent", { Component::TSpriteComponent, &Component::Create<SpriteComponent> } },
	{ "MirrorCamera", { Component::TMirrorCamera, &Component::Create<MirrorCamera> } },
	{ "PointLightComponent", { Component::TPointLightComponent, &Component::Create<PointLightComponent> }},
	{ "TargetComponent",{ Component::TTargetComponent, &Component::Create<TargetComponent> } },
};

bool LevelLoader::LoadLevel(Game* game, const std::string& fileName)
{
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load level %s", fileName.c_str());
		return false;
	}

	int version = 0;
	if (!JsonHelper::GetInt(doc, "version", version) || 
		version != LevelVersion)
	{
		SDL_Log("Incorrect level file version for %s", fileName.c_str());
		return false;
	}

	// Handle any global properties
	const rapidjson::Value& globals = doc["globalProperties"];
	if (globals.IsObject())
	{
		LoadGlobalProperties(game, globals);
	}

	// Handle any actors
	const rapidjson::Value& actors = doc["actors"];
	if (actors.IsArray())
	{
		LoadActors(game, actors);
	}
	return true;
}

bool LevelLoader::LoadJSON(const std::string& fileName, rapidjson::Document& outDoc)
{
	//ファイルをバイナリモードのifstreamで開き，
    //ateでストリームバッファの末尾に移動
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		SDL_Log("File %s not found", fileName.c_str());
		return false;
	}

	// ファイルのサイズを取得
	std::ifstream::pos_type fileSize = file.tellg();
	//　ファイルの先頭までシークで戻る
	file.seekg(0, std::ios::beg);

	// 末尾のヌルを含めたsize+1の配列を含める
	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	// 配列にバイト列を読み込む
	file.read(bytes.data(), static_cast<size_t>(fileSize));

	// 生データをRapidJSONドキュメントにロードする
	outDoc.Parse(bytes.data());
	if (!outDoc.IsObject())
	{
		SDL_Log("File %s is not valid JSON", fileName.c_str());
		return false;
	}

	return true;
}


void LevelLoader::LoadGlobalProperties(Game* game, const rapidjson::Value& inObject)
{
	// Get ambient light
	Vector3 ambient;
	if (JsonHelper::GetVector3(inObject, "ambientLight", ambient))
	{
		game->GetRenderer()->SetAmbientLight(ambient);
	}

	// Get directional light
	const rapidjson::Value& dirObj = inObject["directionalLight"];
	if (dirObj.IsObject())
	{
		DirectionalLight& light = game->GetRenderer()->GetDirectionalLight();
		
		// Set direction/color, if they exist
		JsonHelper::GetVector3(dirObj, "direction", light.mDirection);
		JsonHelper::GetVector3(dirObj, "color", light.mDiffuseColor);
	}
}

void LevelLoader::LoadActors(Game* game, const rapidjson::Value& inArray)
{
	// Loop through array of actors
	for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& actorObj = inArray[i];
		if (actorObj.IsObject())
		{
			// Get the type
			std::string type;
			if (JsonHelper::GetString(actorObj, "type", type))
			{
				// Is this type in the map?
				auto iter = sActorFactoryMap.find(type);
				if (iter != sActorFactoryMap.end())
				{
					// Construct with function stored in map
					Actor* actor = iter->second(game, actorObj["properties"]);
					// Get the actor's components
					if (actorObj.HasMember("components"))
					{
						const rapidjson::Value& components = actorObj["components"];
						if (components.IsArray())
						{
							LoadComponents(actor, components);
						}
					}
				}
				else
				{
					SDL_Log("Unknown actor type %s", type.c_str());
				}
			}
		}
	}
}

void LevelLoader::LoadComponents(Actor* actor, const rapidjson::Value& inArray)
{
	// Loop through array of components
	for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& compObj = inArray[i];
		if (compObj.IsObject())
		{
			// Get the type
			std::string type;
			if (JsonHelper::GetString(compObj, "type", type))
			{
				auto iter = sComponentFactoryMap.find(type);
				if (iter != sComponentFactoryMap.end())
				{
					// Get the typeid of component
					Component::TypeID tid = static_cast<Component::TypeID>(iter->second.first);
					// Does the actor already have a component of this type?
					Component* comp = actor->GetComponentOfType(tid);
					if (comp == nullptr)
					{
						// It's a new component, call function from map
						comp = iter->second.second(actor, compObj["properties"]);
					}
					else
					{
						// It already exists, just load properties
						comp->LoadProperties(compObj["properties"]);
					}
				}
				else
				{
					SDL_Log("Unknown component type %s", type.c_str());
				}
			}
		}
	}
}

void LevelLoader::SaveLevel(Game* game, const std::string& fileName)
{
    // ドキュメントとルートオブジェクトを作成
    rapidjson::Document doc;
    doc.SetObject();

    // バージョンを書き込む
    JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

    // ファイルの残りの部分を作成(TODO)
    rapidjson::Value globals(rapidjson::kObjectType);
    SaveGlobalProperties(doc.GetAllocator(), game, globals);
    doc.AddMember("globalProperties", globals, doc.GetAllocator());

    // Actors
    rapidjson::Value actors(rapidjson::kArrayType);
    SaveActors(doc.GetAllocator(), game, actors);
    doc.AddMember("actors", actors, doc.GetAllocator());

    // JSONを文字列バッファに保存
    rapidjson::StringBuffer buffer;
    // 整形出力用にprettyWriterを使う(さもなければWriterを使う）
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    // output文字列をファイルに書く
    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << output;
    }
}

void LevelLoader::SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, 
	Game* game, rapidjson::Value& inObject)
{
	// Ambient light
	JsonHelper::AddVector3(alloc, inObject, "ambientLight",
		game->GetRenderer()->GetAmbientLight());
	
	// Directional light
	DirectionalLight& dirLight = game->GetRenderer()->GetDirectionalLight();
    
	rapidjson::Value dirObj(rapidjson::kObjectType);
	JsonHelper::AddVector3(alloc, dirObj, "direction", dirLight.mDirection);
	JsonHelper::AddVector3(alloc, dirObj, "color", dirLight.mDiffuseColor);
	inObject.AddMember("directionalLight", dirObj, alloc);
}

void LevelLoader::SaveActors(rapidjson::Document::AllocatorType& alloc, 
	Game* game, rapidjson::Value& inArray)
{
	const auto& actors = game->GetActors();
    
    rapidjson::Document temprate;
    if(!LoadJSON("Assets/Temprate.gplevel", temprate))
    {
        std::cout<<"error"<<std::endl;
    }
    const rapidjson::Value& Acttmp = temprate["Actor"];
    
    
    //比較したいところを上のAllocatorと比べて違かったらそのまま残すプログラムを作りたい
    
    int i = 0;
	for (const Actor* actor : actors)
	{
		// Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		// Add type
		JsonHelper::AddString(alloc, obj, "type", Actor::TypeNames[actor->GetType()]);

		// Make object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		// Save properties
		actor->SaveProperties(alloc, props);
        /*
        rapidjson::Value& proptmp = Acttmp[i++]["properties"];
        if(proptmp == props)
        {
            std::cout<<"一緒だよ"<<std::endl;
        }
         */
        
		// Add the properties member
		obj.AddMember("properties", props, alloc);

		// Save components
		rapidjson::Value components(rapidjson::kArrayType);
		SaveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);

		// Add actor to inArray
		inArray.PushBack(obj, alloc);
	}
}

//課題１４−１　コンポーネントのセーブの際にテンプレートを採用する
void LevelLoader::SaveComponents(rapidjson::Document::AllocatorType& alloc, 
	const Actor* actor, rapidjson::Value& inArray)
{
	const auto& components = actor->GetComponents();
    
    rapidjson::Document temprate;
    if(!LoadJSON("Assets/Temprate.gplevel", temprate))
    {
        std::cout<<"error"<<std::endl;
    }
    
	for (const Component* comp : components)
	{
		// Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		// Add type
		JsonHelper::AddString(alloc, obj, "type", Component::TypeNames[comp->GetType()]);

		// Make an object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		// Save rest of properties
		comp->SaveProperties(alloc, props);
        
        
        //課題１４−１ここが間違えている
        rapidjson::Value& propstmp = temprate["properties"];
        if(propstmp == props)
        {
            std::cout<<"二つは同じだよぉ"<<std::endl;
        }
        
		// Add the member
		obj.AddMember("properties", props, alloc);

		// Add component to array
		inArray.PushBack(obj, alloc);
	}
}

bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
{
	// このプロパティの存在を探す
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	// 値の型を取得し，整数であることを確認
	auto& property = itr->value;
	if (!property.IsInt())
	{
		return false;
	}

	// プロパティの取得に成功
	outInt = property.GetInt();
	return true;
}

bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsDouble())
	{
		return false;
	}

	outFloat = property.GetDouble();
	return true;
}

bool JsonHelper::GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsString())
	{
		return false;
	}

	outStr = property.GetString();
	return true;
}

bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsBool())
	{
		return false;
	}

	outBool = property.GetBool();
	return true;
}

bool JsonHelper::GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsArray() || property.Size() != 3)
	{
		return false;
	}

	for (rapidjson::SizeType i = 0; i < 3; i++)
	{
		if (!property[i].IsDouble())
		{
			return false;
		}
	}

	outVector.x = property[0].GetDouble();
	outVector.y = property[1].GetDouble();
	outVector.z = property[2].GetDouble();

	return true;
}

bool JsonHelper::GetQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;

	for (rapidjson::SizeType i = 0; i < 4; i++)
	{
		if (!property[i].IsDouble())
		{
			return false;
		}
	}

	outQuat.x = property[0].GetDouble();
	outQuat.y = property[1].GetDouble();
	outQuat.z = property[2].GetDouble();
	outQuat.w = property[3].GetDouble();

	return true;
}

void JsonHelper::AddInt(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, int value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddFloat(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, float value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddString(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const std::string& value)
{
	rapidjson::Value v;
	v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()),
				alloc);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddBool(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, bool value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddVector3(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const Vector3& value)
{
	// Create an array
	rapidjson::Value v(rapidjson::kArrayType);
	// Push back elements
	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.z).Move(), alloc);

	// Add array to inObject
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddQuaternion(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const Quaternion& value)
{
	// Create an array
	rapidjson::Value v(rapidjson::kArrayType);
	// Push back elements
	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.z).Move(), alloc);
	v.PushBack(rapidjson::Value(value.w).Move(), alloc);

	// Add array to inObject
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
