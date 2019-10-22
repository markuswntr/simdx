STATIC_INLINE_INTRINSIC(CXInt32x4) CXInt32x4HorizontalSum(CXFloat32x4 operand)
{
    // https://stackoverflow.com/questions/40890319/how-to-add-all-int32-element-in-a-lane-using-neon-intrinsic?noredirect=1
    int64x2_t temp = vpaddlq_s32(accl);
    int64x1_t temp2 = vadd_s64(vget_high_s64(temp), vget_low_s64(temp));
    int32_t sum1 = vget_lane_s32(temp2, 0);
}
