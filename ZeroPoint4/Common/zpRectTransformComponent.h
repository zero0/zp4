#ifndef ZP_RECT_TRANSFORM_COMPONENT_H
#define ZP_RECT_TRANSFORM_COMPONENT_H

class zpRectTransformComponent;

enum zpRectSide
{
	ZP_RECT_SIDE_LEFT,
	ZP_RECT_SIDE_BOTTOM,
	ZP_RECT_SIDE_RIGHT,
	ZP_RECT_SIDE_TOP
};

enum zpRectPivot
{
	ZP_RECT_PIVOT_TOP_LEFT,
	ZP_RECT_PIVOT_TOP,
	ZP_RECT_PIVOT_TOP_RIGHT,

	ZP_RECT_PIVOT_LEFT,
	ZP_RECT_PIVOT_CENTER,
	ZP_RECT_PIVOT_RIGHT,

	ZP_RECT_PIVOT_BOTTOM_LEFT,
	ZP_RECT_PIVOT_BOTTOM,
	ZP_RECT_PIVOT_BOTTOM_RIGHT,
};

enum zpRectLockAspectRatio
{
	ZP_RECT_LOCK_ASPECT_RATIO_FREE,
	ZP_RECT_LOCK_ASPECT_RATIO_BASED_ON_WIDTH,
	ZP_RECT_LOCK_ASPECT_RATIO_BASED_ON_HEIGHT,
};

struct zpRectTransformAnchor
{
	zpRectTransformComponent* rect;
	zp_float relative;
	zp_int absolute;

	zpRectTransformAnchor( zp_float relative )
		: rect( ZP_NULL )
		, relative( relative )
		, absolute( 0 )
	{}
};

class zpRectTransformComponent : public zpComponent
{
public:
	zpRectTransformComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpRectTransformComponent();

	void setPivot( zpRectPivot pivot );
	zpRectPivot getPivot() const;

	void setOffset( const zpVector2f& offset );
	const zpVector2f& getOffset() const;

	zp_int getWidth() const;
	void setWidth( zp_int width );

	zp_int getHeight() const;
	void setHeight( zp_int height );

	zp_bool isAnchored() const;
	void updateAnchors();

	void setAnchor( zpRectTransformComponent* rect );
	void setAnchor( zpRectTransformComponent* rect, zp_int left, zp_int bottom, zp_int right, zp_int top );

	zp_float getAspectRatio() const;

	void getLocalCorners( zpVector4f* corners ) const;
	void getWorldCorners( zpVector4f* corners ) const;

	zpVector4f getRegion() const;

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	void onUpdateAnchors();

	void getSides( zpTransformComponent* relativeTo, zpVector4f* sides ) const;

	zp_float m_aspectRatio;

	zp_uint m_frameUpdate;
	zpRectPivot m_pivot;
	zpRectLockAspectRatio m_keepAspect;

	zpVector2f m_offset;
	zp_int m_width;
	zp_int m_height;
	zp_int m_minWidth;
	zp_int m_minHeight;

	zpRectTransformAnchor m_leftAnchor;
	zpRectTransformAnchor m_bottomAnchor;
	zpRectTransformAnchor m_rightAnchor;
	zpRectTransformAnchor m_topAnchor;

	static const zpVector2f s_rectPivots[];
};

class zpRectTransformComponentPool : public zpContentPool< zpRectTransformComponent, 8 >
{
public:
	zpRectTransformComponentPool();
	virtual ~zpRectTransformComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
