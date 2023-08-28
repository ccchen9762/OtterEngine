#include "Character.h"

Character::Character(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
	bool isStatic, const std::string& path) : Model(game, graphics, translation, rotation, scale, isStatic, path){

}
