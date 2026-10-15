/* Canonical interfaces used by the display-management implementations. */
#ifndef DOVI_DM_BINDINGS_H
#define DOVI_DM_BINDINGS_H
#include "fixed_access.h"
#include "dm_pipeline.h"
#include "dm_configuration.h"
#include "dm_frame_format.h"
#include "dm_transfer.h"
#include "color_basis.h"
#include "matrix_arithmetic.h"
#include "color_narrow.h"
#include "color_primaries.h"
#include "numeric_color_matrix.h"
#include "fixed_arithmetic.h"
#include "fixed_transcendentals.h"
#include "pq_conversion.h"
#include "numeric_transfer.h"
#include "numeric_helpers.h"
#include "tone_curve.h"
#include "control_memory.h"
#include "numeric_common.h"
#include "shared_data.h"
#include "kernel_map_composition.h"
#include "tone_mapping_math.h"

#define MemCpyByte dovi_MemCpyByte
#define LToPQ12 dovi_LToPQ12
#define GetLms2IptM33Fxp dovi_GetLms2IptM33Fxp
#define GetIpt2LmsM33Fxp dovi_GetIpt2LmsM33Fxp
#define InvM33 dovi_InvM33
#define AssignM33I2I dovi_AssignM33I2I
#define AssignV3I2I dovi_AssignV3I2I
#define Yuv2RgbOffsetRgbFxp dovi_Yuv2RgbOffsetRgbFxp
#define GetRgb2LmsCtWpM33Fxp dovi_GetRgb2LmsCtWpM33Fxp
#define GetYuv2RgbM33NarrowFxp dovi_GetYuv2RgbM33NarrowFxp
#define GetYuv2RgbM33Fxp dovi_GetYuv2RgbM33Fxp
#define GetYuvRgbOffFxp dovi_GetYuvRgbOffFxp
#define GetRange dovi_GetRange
#define CreateToneCurve dovi_CreateToneCurve

#define APPLY_CT2RIGHT dovi_APPLY_CT2RIGHT
#define Adjust64M33ScaleTo16M33 dovi_Adjust64M33ScaleTo16M33
#define ApplyLocalMs dovi_ApplyLocalMs
#define Cct2Xy dovi_Cct2Xy
#define CommitKsDs dovi_CommitKsDs
#define CommitKsUs dovi_CommitKsUs
#define ConcatenatePriBwdMapping dovi_ConcatenatePriBwdMapping
#define DeriveHlgEotfParam dovi_DeriveHlgEotfParam
#define GetCtM33 dovi_GetCtM33
#define GetCvmBypassDnParam dovi_GetCvmBypassDnParam
#define GetCvmBypassNormParam dovi_GetCvmBypassNormParam
#define GetDisplay dovi_GetDisplay
#define GetGammaABG dovi_GetGammaABG
#define GetIctcpIo2LmsM33 dovi_GetIctcpIo2LmsM33
#define GetIctcpIo2LmsM33Narrow dovi_GetIctcpIo2LmsM33Narrow
#define GetIntendedWpCct dovi_GetIntendedWpCct
#define GetIpt2LmsM33NarrowFxp dovi_GetIpt2LmsM33NarrowFxp
#define GetLms2IctcpIoM33 dovi_GetLms2IctcpIoM33
#define GetLms2IctcpIoM33Narrow dovi_GetLms2IctcpIoM33Narrow
#define GetLms2IptM33NarrowFxp dovi_GetLms2IptM33NarrowFxp
#define GetLms2RgbByV8PrimsM33Fxp dovi_GetLms2RgbByV8PrimsM33Fxp
#define GetLms2RgbCtWpM33Fxp dovi_GetLms2RgbCtWpM33Fxp
#define GetPrimaries dovi_GetPrimaries
#define GetRgb2LmsByV8PrimsM33Fxp dovi_GetRgb2LmsByV8PrimsM33Fxp
#define GetRgb2LmsM33 dovi_GetRgb2LmsM33
#define GetRgb2XyzDiagByV8Prims dovi_GetRgb2XyzDiagByV8Prims
#define GetRgb2XyzM33ByPrims dovi_GetRgb2XyzM33ByPrims
#define GetRgb2XyzM33ByV8Prims dovi_GetRgb2XyzM33ByV8Prims
#define GetRgb2YuvM33Fxp dovi_GetRgb2YuvM33Fxp
#define GetRgb2YuvM33NarrowFxp dovi_GetRgb2YuvM33NarrowFxp
#define GetXyz2LmsM33 dovi_GetXyz2LmsM33
#define L2PqX dovi_L2PqX
#define MemEqByte dovi_MemEqByte
#define PQ12ToL dovi_PQ12ToL
#define Pq2L dovi_Pq2L
#define Pq2LX dovi_Pq2LX
#define RE_CT2RIGHT_M33 dovi_RE_CT2RIGHT_M33
#define RegMemReq dovi_RegMemReq
#define SortAscend dovi_SortAscend
#define Xy2Xyz dovi_Xy2Xyz
#define Xyz2IctcpDm dovi_Xyz2IctcpDm
#define fxLOG2Taylor dovi_fxLOG2Taylor
#define sdiv dovi_sdiv
#define ParseMds dovi_dm_ParseMds
#define GetMdsOutput dovi_dm_GetMdsOutput
#define GenerateOutputMd dovi_dm_GenerateOutputMd
#define FindTrimIdxUsed dovi_dm_FindTrimIdxUsed
#define GetTMaxFromTrim dovi_dm_GetTMaxFromTrim
#define BuildLumaXInfo dovi_dm_BuildLumaXInfo
#define BuildChromaXInfo dovi_dm_BuildChromaXInfo
#define Commit2DmKs dovi_dm_Commit2DmKs
#define ConsolidateInput dovi_dm_ConsolidateInput
#define ApplyAblu dovi_dm_ApplyAblu
#define CommitFinalized dovi_dm_CommitFinalized
#define memset dovi_control_os.memset
#define _printk dovi_control_os.printk
#define dump_stack dovi_control_os.dump_stack
/* Named immutable data shared by the display-management stages. */
#define dovi_dm_assertion_format (dovi_retained_section_3.diagnostics.assertion_format)
#define dovi_dm_assertion_false (dovi_retained_section_3.diagnostics.assertion_false)
#define dovi_v8_primaries (dovi_retained_section_3.version8_primaries)
#define dovi_level2_luma_trim (dovi_retained_section_3.level2_luma_trim)
#define dovi_level8_luma_trim (dovi_retained_section_3.level8_luma_trim)
#define dovi_level2_chroma_trim (dovi_retained_section_3.level2_chroma_trim)
#define dovi_level8_chroma_trim (dovi_retained_section_3.level8_chroma_trim)
#define dovi_pulsar_gamma_to_linear (dovi_retained_section_3.pulsar_gamma_to_linear)
#define dovi_sdr_gamma_to_linear (dovi_retained_section_3.sdr_gamma_to_linear)
#define dovi_log10_value (dovi_retained_section_3.log10_value)
#define dovi_log10_rate (dovi_retained_section_3.log10_rate)
#define dovi_power10_value (dovi_retained_section_3.power10_value)
#define dovi_power10_scale (dovi_retained_section_3.power10_scale)
#endif
