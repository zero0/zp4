#include "zpCommon.h"

zpGameManager::zpGameManager() 
	: m_flags( 0 )
	, m_game( ZP_NULL )
{}
zpGameManager::~zpGameManager() {}

void zpGameManager::create() {
	if( m_flags.isMarked( ZP_GAME_MANAGER_FLAG_CREATED ) ) return;
	m_flags.mark( ZP_GAME_MANAGER_FLAG_CREATED );
	
	onCreate();
}
void zpGameManager::destroy() {
	if( !m_flags.isMarked( ZP_GAME_MANAGER_FLAG_CREATED ) ) return;
	m_flags.unmark( ZP_GAME_MANAGER_FLAG_CREATED );

	onDestroy();
}

void zpGameManager::update() {
	if( m_flags.isAllMarked( 1 << ZP_GAME_MANAGER_FLAG_CREATED | 1 << ZP_GAME_MANAGER_FLAG_ENABLED ) ) onUpdate();
}
void zpGameManager::simulate() {
	if( m_flags.isAllMarked( 1 << ZP_GAME_MANAGER_FLAG_CREATED | 1 << ZP_GAME_MANAGER_FLAG_ENABLED ) ) onSimulate();
}

void zpGameManager::setEnabled( zp_bool enabled ) {
	bool wasEnabled = m_flags.isMarked( ZP_GAME_MANAGER_FLAG_ENABLED );
	m_flags.setMarked( ZP_GAME_MANAGER_FLAG_ENABLED, enabled );

	if( wasEnabled && !enabled ) {
		onDisabled();
	} else if( !wasEnabled && enabled ) {
		onEnabled();
	}
}
zp_bool zpGameManager::isEnabled() const {
	return m_flags.isMarked( ZP_GAME_MANAGER_FLAG_ENABLED );
}

void zpGameManager::setGame( zpGame* game ) {
	m_game = game;
}
zpGame* zpGameManager::getGame() const {
	return m_game;
}
