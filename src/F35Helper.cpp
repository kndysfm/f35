#include "stdafx.h"

#include "F35Helper.h"

D2D1_RECT_F F35_NS::MakeRectRatios(D2D1_RECT_F src_rect, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	D2D1_RECT_F new_rect;

	new_rect.left = left * src_rect.right + (1.0f - left) * src_rect.left;
	new_rect.top = top * src_rect.bottom + (1.0f - top) * src_rect.top;
	new_rect.right = right * src_rect.right + (1.0f - right) * src_rect.left;
	new_rect.bottom = bottom * src_rect.bottom + (1.0f - bottom) * src_rect.top;

	return new_rect;
}

D2D1_RECT_F F35_NS::MakeRectRatios(D2D1_RECT_F src_rect, D2D1_RECT_F ratio_rect)
{
	return MakeRectRatios(src_rect, ratio_rect.left, ratio_rect.top, ratio_rect.right, ratio_rect.bottom);
}

D2D1_POINT_2F F35_NS::MakePointRatio(D2D1_POINT_2F pt0, D2D1_POINT_2F pt1, FLOAT ratio)
{
	D2D1_POINT_2F new_pt;

	new_pt.x = ratio * pt1.x, +(1.0f - ratio) * pt0.x;
	new_pt.y = ratio * pt1.y, +(1.0f - ratio) * pt0.y;

	return new_pt;
}
