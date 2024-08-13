#include "Level.h"
#include "GameObject.h"

void Level::OnDestroyed()
{
	for (ObjectHandle<GameObject> object : _levelObjects)
	{
		ObjectRef<GameObject> objectRef = object.lock();
		if (objectRef)
		{
			objectRef->Destroy();
		}
	}
}
