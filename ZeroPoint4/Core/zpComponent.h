#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

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

	void create();
	void destroy();

	const zpString& getName() const;
	void setName( const zpString& name );

	void sendMessageToParentGameObject( const zpMessage& message );
	void sendMessageToSiblingComponents( const zpMessage& message );

protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpGameObject* m_parentGameObject;

	zpString m_name;
};

#endif