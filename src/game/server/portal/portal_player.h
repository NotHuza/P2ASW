//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef PORTAL_PLAYER_H
#define PORTAL_PLAYER_H
#pragma once

class CPortal_Player;

#include "player_pickup_controller.h"
#include "player.h"
#include "portal_playeranimstate.h"
#include "hl2_playerlocaldata.h"
#include "hl2_player.h"
#include "simtimer.h"
#include "soundenvelope.h"
#include "portal_player_shared.h"
#include "prop_portal.h"
#include "weapon_portalbase.h"
#include "in_buttons.h"
#include "func_liquidportal.h"
#include "ai_speech.h"			// For expresser host
#include "paint/paint_power_user.h"
#include "paint/paintable_entity.h"
#include "portal_playerlocaldata.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterialvar.h"

struct PortalPlayerStatistics_t
{
	int iNumPortalsPlaced;
	int iNumStepsTaken;
	float fNumSecondsTaken;
};



//=============================================================================
// >> Portal_Player
//=============================================================================
class CPortal_Player : public PaintPowerUser< CPaintableEntity< CHL2_Player > >
{
public:
	DECLARE_CLASS( CPortal_Player, PaintPowerUser< CPaintableEntity< CHL2_Player > > );

	CPortal_Player();
	~CPortal_Player( void );
	
	static CPortal_Player *CreatePlayer( const char *className, edict_t *ed )
	{
		CPortal_Player::s_PlayerEdict = ed;
		return (CPortal_Player*)CreateEntityByName( className );
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void Precache( void );
	virtual void CreateSounds( void );
	virtual void StopLoopingSounds( void );
	virtual void Spawn( void );
	virtual void OnRestore( void );
	virtual void Activate( void );

	virtual void NotifySystemEvent( CBaseEntity *pNotify, notify_system_event_t eventType, const notify_system_event_params_t &params );

	virtual void PostThink( void );
	virtual void PreThink( void );
	virtual void PlayerDeathThink( void );

	void UpdatePortalPlaneSounds( void );
	void UpdateWooshSounds( void );

	Activity TranslateActivity( Activity ActToTranslate, bool *pRequired = NULL );
	virtual void Teleport( const Vector *newPosition, const QAngle *newAngles, const Vector *newVelocity );

	Activity TranslateTeamActivity( Activity ActToTranslate );

	virtual void SetAnimation( PLAYER_ANIM playerAnim );

	virtual void PlayerRunCommand(CUserCmd *ucmd, IMoveHelper *moveHelper);

	virtual bool ClientCommand( const CCommand &args );
	virtual void CreateViewModel( int viewmodelindex = 0 );
	virtual bool BecomeRagdollOnClient( const Vector &force );
	virtual int	OnTakeDamage( const CTakeDamageInfo &inputInfo );
	virtual int	OnTakeDamage_Alive( const CTakeDamageInfo &info );
	virtual bool WantsLagCompensationOnEntity( const CBasePlayer *pPlayer, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits ) const;
	virtual void FireBullets ( const FireBulletsInfo_t &info );
	virtual bool Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex = 0);
	virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon );
	virtual void ShutdownUseEntity( void );

	virtual const Vector&	WorldSpaceCenter( ) const;

	virtual void VPhysicsShadowUpdate( IPhysicsObject *pPhysics );

	//virtual bool StartReplayMode( float fDelay, float fDuration, int iEntity  );
	//virtual void StopReplayMode();
 	virtual void Event_Killed( const CTakeDamageInfo &info );
	virtual void Jump( void );

	bool UseFoundEntity( CBaseEntity *pUseEntity );
	CBaseEntity* FindUseEntity( void );
	CBaseEntity* FindUseEntityThroughPortal( void );

	virtual void PlayerUse( void );
	//virtual bool StartObserverMode( int mode );
	virtual void GetStepSoundVelocities( float *velwalk, float *velrun );
	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force );
	virtual void UpdateOnRemove( void );

	virtual void SetupVisibility( CBaseEntity *pViewEntity, unsigned char *pvs, int pvssize );
	virtual void UpdatePortalViewAreaBits( unsigned char *pvs, int pvssize );
	
	bool	ValidatePlayerModel( const char *pModel );

	QAngle GetAnimEyeAngles( void ) { return m_angEyeAngles.Get(); }

	Vector GetAttackSpread( CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL );

	void CheatImpulseCommands( int iImpulse );
	void CreateRagdollEntity( const CTakeDamageInfo &info );
	void GiveAllItems( void );
	void GiveDefaultItems( void );

	void NoteWeaponFired( void );

	void ResetAnimation( void );

	void SetPlayerModel( void );
	
	int	  GetPlayerModelType( void ) { return m_iPlayerSoundType; }

	void ForceDuckThisFrame( void );
	void UnDuck ( void );
	inline void ForceJumpThisFrame( void ) { ForceButtons( IN_JUMP ); }

	void DoAnimationEvent( PlayerAnimEvent_t event, int nData );
	void SetupBones( matrix3x4_t *pBoneToWorld, int boneMask );

	// physics interactions
	virtual void PickupObject(CBaseEntity *pObject, bool bLimitMassAndSize );
	virtual void ForceDropOfCarriedPhysObjects( CBaseEntity *pOnlyIfHoldingThis );

	void ToggleHeldObjectOnOppositeSideOfPortal( void ) { m_bHeldObjectOnOppositeSideOfPortal = !m_bHeldObjectOnOppositeSideOfPortal; }
	void SetHeldObjectOnOppositeSideOfPortal( bool p_bHeldObjectOnOppositeSideOfPortal ) { m_bHeldObjectOnOppositeSideOfPortal = p_bHeldObjectOnOppositeSideOfPortal; }
	bool IsHeldObjectOnOppositeSideOfPortal( void ) { return m_bHeldObjectOnOppositeSideOfPortal; }
	CProp_Portal *GetHeldObjectPortal( void ) { return m_pHeldObjectPortal; }
	void SetHeldObjectPortal( CProp_Portal *pPortal ) { m_pHeldObjectPortal = pPortal; }

	void SetStuckOnPortalCollisionObject( void ) { m_bStuckOnPortalCollisionObject = true; }

	CWeaponPortalBase* GetActivePortalWeapon() const;

	void IncrementPortalsPlaced( void );
	void IncrementStepsTaken( void );
	void UpdateSecondsTaken( void );
	void ResetThisLevelStats( void );
	int NumPortalsPlaced( void ) const { return m_StatsThisLevel.iNumPortalsPlaced; }
	int NumStepsTaken( void ) const { return m_StatsThisLevel.iNumStepsTaken; }
	float NumSecondsTaken( void ) const { return m_StatsThisLevel.fNumSecondsTaken; }

	void SetNeuroToxinDamageTime( float fCountdownSeconds ) { m_fNeuroToxinDamageTime = gpGlobals->curtime + fCountdownSeconds; }

	void IncNumCamerasDetatched( void ) { ++m_iNumCamerasDetatched; }
	int GetNumCamerasDetatched( void ) const { return m_iNumCamerasDetatched; }

	Vector m_vecTotalBulletForce;	//Accumulator for bullet force in a single frame

	bool m_bSilentDropAndPickup;

	// Tracks our ragdoll entity.
	CNetworkHandle( CBaseEntity, m_hRagdoll );	// networked entity handle
	
	void	SetMotionBlurAmount( float flAmt ) { m_flMotionBlurAmount = flAmt; }
	
	const CPortalPlayerLocalData& GetPortalPlayerLocalData() const;
	
	CGrabController m_GrabController;

	CGrabController &GetGrabController()
	{
		return m_GrabController;
	}


protected:

	CNetworkVarEmbedded( CPortalPlayerLocalData, m_PortalLocal );
		
private:

	CNetworkVar( float,  m_flMotionBlurAmount );

	CSoundPatch		*m_pWooshSound;

	CNetworkQAngle( m_angEyeAngles );

	CPortalPlayerAnimState*   m_PlayerAnimState;

	int m_iLastWeaponFireUsercmd;
	CNetworkVar( int, m_iSpawnInterpCounter );
	CNetworkVar( int, m_iPlayerSoundType );

	CNetworkVar( bool, m_bHeldObjectOnOppositeSideOfPortal );
	CNetworkHandle( CProp_Portal, m_pHeldObjectPortal );	// networked entity handle

	bool m_bIntersectingPortalPlane;
	bool m_bStuckOnPortalCollisionObject;

	float m_fTimeLastHurt;
	bool  m_bIsRegenerating;		// Is the player currently regaining health

	float m_fNeuroToxinDamageTime;

	PortalPlayerStatistics_t m_StatsThisLevel;
	float m_fTimeLastNumSecondsUpdate;

	int		m_iNumCamerasDetatched;

	QAngle						m_qPrePortalledViewAngles;
	bool						m_bFixEyeAnglesFromPortalling;
	VMatrix						m_matLastPortalled;

	

	mutable Vector m_vWorldSpaceCenterHolder; //WorldSpaceCenter() returns a reference, need an actual value somewhere
	
public: // PAINT SPECIFIC

	Vector GetPaintGunShootPosition();
	
	bool IsPressingJumpKey() const;
	bool IsHoldingJumpKey() const;
	bool IsTryingToSuperJump( const PaintPowerInfo_t* pInfo = NULL ) const;
	void SetJumpedThisFrame( bool jumped );
	bool JumpedThisFrame() const;
	void SetBouncedThisFrame( bool bounced );
	bool BouncedThisFrame() const;
	InAirState GetInAirState() const;
	
	bool WantsToSwapGuns( void );
	void SetWantsToSwapGuns( bool bWantsToSwap );

	virtual PaintPowerType GetPaintPowerAtPoint( const Vector& worldContactPt ) const;
	virtual void Paint( PaintPowerType type, const Vector& worldContactPt );
	virtual void CleansePaint();

	void Reorient( QAngle& viewAngles );
	float GetReorientationProgress() const;
	bool IsDoneReorienting() const;
	
	virtual const Vector GetPlayerMins() const;
	virtual const Vector GetPlayerMaxs() const;
	const Vector& GetHullMins() const;
	const Vector& GetHullMaxs() const;
	const Vector& GetStandHullMins() const;
	const Vector& GetStandHullMaxs() const;
	const Vector& GetDuckHullMins() const;
	const Vector& GetDuckHullMaxs() const;

	float GetHullHeight() const;
	float GetHullWidth() const;
	float GetStandHullHeight() const;
	float GetStandHullWidth() const;
	float GetDuckHullHeight() const;
	float GetDuckHullWidth() const;
	
	virtual void UpdateCollisionBounds();
	virtual void InitVCollision( const Vector &vecAbsOrigin, const Vector &vecAbsVelocity );
	
	const Vector& GetInputVector() const;
	void SetInputVector( const Vector& vInput );

	// stick camera
	void RotateUpVector( Vector& vForward, Vector& vUp );
	void SnapCamera( StickCameraState nCameraState, bool bLookingInBadDirection );
	//void PostTeleportationCameraFixup( const CProp_Portal *pEnteredPortal );

	StickCameraState GetStickCameraState() const;
	void SetQuaternionPunch( const Quaternion& qPunch );
	void DecayQuaternionPunch();
	
	using BaseClass::AddSurfacePaintPowerInfo;
	void AddSurfacePaintPowerInfo( const BrushContact& contact, char const* context = 0 );
	void AddSurfacePaintPowerInfo( const trace_t& trace, char const* context = 0 );
	
	void SetEyeOffset( const Vector& vOldOrigin, const Vector& vNewOrigin );
	
	void SetHullHeight( float flHeight );
	
	void OnBounced( float fTimeOffset = 0.0f );
	
	virtual void ChooseActivePaintPowers( PaintPowerInfoVector& activePowers );
	
	bool IsFullyConnected() { return m_bIsFullyConnected; }

private: // PAINT SPECIFIC

	void DecayEyeOffset();
	
	// Find all the contacts
	void DeterminePaintContacts();
	void PredictPaintContacts( const Vector& contactBoxMin,
								const Vector& contactBoxMax,
								const Vector& traceBoxMin,
								const Vector& traceBoxMax,
								float lookAheadTime,
								char const* context );
	void ChooseBestPaintPowersInRange( PaintPowerChoiceResultArray& bestPowers,
										PaintPowerConstIter begin,
										PaintPowerConstIter end,
										const PaintPowerChoiceCriteria_t& info ) const;
	
	// Paint Power User Implementation
	virtual PaintPowerState ActivateSpeedPower( PaintPowerInfo_t& powerInfo );
	virtual PaintPowerState UseSpeedPower( PaintPowerInfo_t& powerInfo );
	virtual PaintPowerState DeactivateSpeedPower( PaintPowerInfo_t& powerInfo );

	virtual PaintPowerState ActivateBouncePower( PaintPowerInfo_t& powerInfo );
	virtual PaintPowerState UseBouncePower( PaintPowerInfo_t& powerInfo );
	virtual PaintPowerState DeactivateBouncePower( PaintPowerInfo_t& powerInfo );
	
	void PlayPaintSounds( const PaintPowerChoiceResultArray& touchedPowers );
	void UpdatePaintedPower();
	void UpdateAirInputScaleFadeIn();
	void UpdateInAirState();
	void CachePaintPowerChoiceResults( const PaintPowerChoiceResultArray& choiceInfo );
	bool LateSuperJumpIsValid() const;
	void RecomputeBoundsForOrientation();
	void TryToChangeCollisionBounds( const Vector& newStandHullMin,
		const Vector& newStandHullMax,
		const Vector& newDuckHullMin,
		const Vector& newDuckHullMax );
	
	float SpeedPaintAcceleration( float flDefaultMaxSpeed,
		float flSpeed,
		float flWishCos,
		float flWishDirSpeed ) const;
	
	bool CheckToUseBouncePower( PaintPowerInfo_t& info );
	
	// PAINT POWER STATE
	PaintPowerInfo_t m_CachedJumpPower;
	Vector m_vInputVector;
	float m_flCachedJumpPowerTime;
	float m_flUsePostTeleportationBoxTime;
	float m_flSpeedDecelerationTime;
	float m_flPredictedJumpTime;
	bool m_bJumpWasPressedWhenForced;	// The jump button was actually pressed when ForceDuckThisFrame() was called
	int m_nBounceCount;	// Number of bounces in a row without touching the ground
	float m_LastGroundBouncePlaneDistance;
	float m_flLastSuppressedBounceTime;
	float m_flTimeSinceLastTouchedPower[3];
	int m_nPortalsEnteredInAirFlags;
	int m_nAirTauntCount;
		
	CNetworkVar( float, m_flHullHeight );
	//Swapping guns
	CNetworkVar( bool, m_bWantsToSwapGuns );
	bool m_bSendSwapProximityFailEvent;
	
	// Paint power debug
	void DrawJumpHelperDebug( PaintPowerConstIter begin, PaintPowerConstIter end, float duration, bool noDepthTest, const PaintPowerInfo_t* pSelected ) const;

	bool m_bIsFullyConnected;



public:

	CNetworkVar( bool, m_bPitchReorientation );
	CNetworkHandle( CProp_Portal, m_hPortalEnvironment ); //if the player is in a portal environment, this is the associated portal
	CNetworkHandle( CFunc_LiquidPortal, m_hSurroundingLiquidPortal ); //if the player is standing in a liquid portal, this will point to it

	friend class CProp_Portal;


#ifdef PORTAL_MP
public:
	virtual CBaseEntity* EntSelectSpawnPoint( void );
	void PickTeam( void );
#endif
};

inline CPortal_Player *ToPortalPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CPortal_Player*>( pEntity );
}

inline CPortal_Player *GetPortalPlayer( int iPlayerIndex )
{
	return static_cast<CPortal_Player*>( UTIL_PlayerByIndex( iPlayerIndex ) );
}

#endif //PORTAL_PLAYER_H