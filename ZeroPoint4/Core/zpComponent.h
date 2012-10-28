#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

class zpWorld;
class zpGame;

ZP_ABSTRACT_CLASS zpComponent : 
	public zpIntrusiveListNode<zpComponent>,
	public zpReferencedObject, 
	public zpMessageReceiver,
	public zpSerializable
{
public:
	zpComponent();
	virtual ~zpComponent();

	void setParentGameObject( zpGameObject* parent );
	zpGameObject* getParentGameObject() const;

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;
	zp_bool isCreated() const;

	void update();
	void simulate();

	void create();
	void destroy();

	const zpString& getName() const;
	void setName( const zpString& name );

	void sendMessageToParentGameObject( const zpMessage& message );
	void sendMessageToSiblingComponents( const zpMessage& message );

	zpWorld* getWorld() const;
	zpGame* getGame() const;

	template<typename T>
	T* getGameManagerOfType() const {
		return getGame()->getGameManagerOfType<T>();
	}

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;
	virtual void onSimulate() {};

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	struct {
		mutable zp_uint m_referenceCount : 31;
		mutable zp_bool m_isMarkedForAutoDelete : 1;
	};

	zpGameObject* m_parentGameObject;

	zpString m_name;
};

#endif