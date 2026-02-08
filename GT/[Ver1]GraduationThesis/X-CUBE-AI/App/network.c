/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-02-08T21:58:45+0700
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "0x97fd31a39dee7175162a509e4352903c"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-02-08T21:58:45+0700"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 12987, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  _Reshape_output_0_to_chfirst_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12987, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 53440, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  _stem_stem_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 53440, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13440, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 26880, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_relu_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 26880, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6720, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13440, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_relu_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13440, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4032, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8064, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_relu_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8064, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  output_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 2, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2048, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8192, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  output_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  output_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 9, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  _Reshape_output_0_to_chfirst_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 333, 39), AI_STRIDE_INIT(4, 4, 4, 4, 1332),
  1, &_Reshape_output_0_to_chfirst_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_bias, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_features_features_0_depthwise_Conv_output_0_bias_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 84, 10), AI_STRIDE_INIT(4, 4, 4, 64, 5376),
  1, &_features_features_0_depthwise_Conv_output_0_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_weights, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &_features_features_0_depthwise_Conv_output_0_weights_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_features_features_0_pointwise_Conv_output_0_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 84, 10), AI_STRIDE_INIT(4, 4, 4, 128, 10752),
  1, &_features_features_0_pointwise_Conv_output_0_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_scratch0, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_features_features_0_pointwise_Conv_output_0_scratch0_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_weights, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 32), AI_STRIDE_INIT(4, 4, 64, 2048, 2048),
  1, &_features_features_0_pointwise_Conv_output_0_weights_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_0_relu_Clip_output_0_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 84, 10), AI_STRIDE_INIT(4, 4, 4, 128, 10752),
  1, &_features_features_0_relu_Clip_output_0_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_bias, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_features_features_2_depthwise_Conv_output_0_bias_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 42, 5), AI_STRIDE_INIT(4, 4, 4, 128, 5376),
  1, &_features_features_2_depthwise_Conv_output_0_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_weights, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 32), AI_STRIDE_INIT(4, 1, 32, 32, 32),
  1, &_features_features_2_depthwise_Conv_output_0_weights_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_bias, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &_features_features_2_pointwise_Conv_output_0_bias_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 42, 5), AI_STRIDE_INIT(4, 4, 4, 256, 10752),
  1, &_features_features_2_pointwise_Conv_output_0_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_scratch0, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_features_features_2_pointwise_Conv_output_0_scratch0_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_weights, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 32, 1, 1, 64), AI_STRIDE_INIT(4, 4, 128, 8192, 8192),
  1, &_features_features_2_pointwise_Conv_output_0_weights_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_2_relu_Clip_output_0_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 42, 5), AI_STRIDE_INIT(4, 4, 4, 256, 10752),
  1, &_features_features_2_relu_Clip_output_0_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_bias, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &_features_features_4_depthwise_Conv_output_0_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_output, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 21, 3), AI_STRIDE_INIT(4, 4, 4, 256, 5376),
  1, &_features_features_4_depthwise_Conv_output_0_output_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_weights, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 64), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &_features_features_4_depthwise_Conv_output_0_weights_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_bias, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_features_features_4_pointwise_Conv_output_0_bias_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_output, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 21, 3), AI_STRIDE_INIT(4, 4, 4, 512, 10752),
  1, &_features_features_4_pointwise_Conv_output_0_output_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_scratch0, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &_features_features_4_pointwise_Conv_output_0_scratch0_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_weights, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 128), AI_STRIDE_INIT(4, 4, 256, 32768, 32768),
  1, &_features_features_4_pointwise_Conv_output_0_weights_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  _features_features_4_relu_Clip_output_0_output, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 21, 3), AI_STRIDE_INIT(4, 4, 4, 512, 10752),
  1, &_features_features_4_relu_Clip_output_0_output_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_output, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_pool_GlobalAveragePool_output_0_output_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_bias, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_stem_stem_0_Conv_output_0_bias_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_output, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 167, 20), AI_STRIDE_INIT(4, 4, 4, 64, 10688),
  1, &_stem_stem_0_Conv_output_0_output_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_scratch0, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 3, 3), AI_STRIDE_INIT(4, 4, 4, 4, 12),
  1, &_stem_stem_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_weights, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &_stem_stem_0_Conv_output_0_weights_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  _stem_stem_2_Clip_output_0_output, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 167, 20), AI_STRIDE_INIT(4, 4, 4, 64, 10688),
  1, &_stem_stem_2_Clip_output_0_output_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 333, 1, 39), AI_STRIDE_INIT(4, 4, 4, 1332, 1332),
  1, &input_output_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  input_output0, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 333, 39, 1), AI_STRIDE_INIT(4, 4, 4, 1332, 51948),
  1, &input_output_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  output_bias, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &output_bias_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  output_output, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &output_output_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  output_weights, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 128, 2, 1, 1), AI_STRIDE_INIT(4, 4, 512, 1024, 1024),
  1, &output_weights_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  output_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_GlobalAveragePool_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &output_weights, &output_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  output_layer, 44,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &output_chain,
  NULL, &output_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_4_relu_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_GlobalAveragePool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_layer, 42,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap,
  &_pool_GlobalAveragePool_output_0_chain,
  NULL, &output_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(21, 3), 
  .pool_stride = AI_SHAPE_2D_INIT(21, 3), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)


AI_STATIC_CONST ai_float _features_features_4_relu_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _features_features_4_relu_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _features_features_4_relu_Clip_output_0_nl_params_data, _features_features_4_relu_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_4_relu_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_4_pointwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_4_relu_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _features_features_4_relu_Clip_output_0_layer, 41,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_features_features_4_relu_Clip_output_0_chain,
  NULL, &_pool_GlobalAveragePool_output_0_layer, AI_STATIC, 
  .nl_params = &_features_features_4_relu_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_4_depthwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_4_pointwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_features_features_4_pointwise_Conv_output_0_weights, &_features_features_4_pointwise_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_features_features_4_pointwise_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _features_features_4_pointwise_Conv_output_0_layer, 38,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_features_features_4_pointwise_Conv_output_0_chain,
  NULL, &_features_features_4_relu_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_2_relu_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_4_depthwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_features_features_4_depthwise_Conv_output_0_weights, &_features_features_4_depthwise_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _features_features_4_depthwise_Conv_output_0_layer, 37,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_features_features_4_depthwise_Conv_output_0_chain,
  NULL, &_features_features_4_pointwise_Conv_output_0_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _features_features_2_relu_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _features_features_2_relu_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _features_features_2_relu_Clip_output_0_nl_params_data, _features_features_2_relu_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_2_relu_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_2_pointwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_2_relu_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _features_features_2_relu_Clip_output_0_layer, 36,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_features_features_2_relu_Clip_output_0_chain,
  NULL, &_features_features_4_depthwise_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_features_features_2_relu_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_2_depthwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_2_pointwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_features_features_2_pointwise_Conv_output_0_weights, &_features_features_2_pointwise_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_features_features_2_pointwise_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _features_features_2_pointwise_Conv_output_0_layer, 33,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_features_features_2_pointwise_Conv_output_0_chain,
  NULL, &_features_features_2_relu_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_0_relu_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_2_depthwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_features_features_2_depthwise_Conv_output_0_weights, &_features_features_2_depthwise_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _features_features_2_depthwise_Conv_output_0_layer, 32,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_features_features_2_depthwise_Conv_output_0_chain,
  NULL, &_features_features_2_pointwise_Conv_output_0_layer, AI_STATIC, 
  .groups = 32, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _features_features_0_relu_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _features_features_0_relu_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _features_features_0_relu_Clip_output_0_nl_params_data, _features_features_0_relu_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_0_relu_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_0_pointwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_0_relu_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _features_features_0_relu_Clip_output_0_layer, 31,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_features_features_0_relu_Clip_output_0_chain,
  NULL, &_features_features_2_depthwise_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_features_features_0_relu_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_0_depthwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_0_pointwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_features_features_0_pointwise_Conv_output_0_weights, &_features_features_0_pointwise_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_features_features_0_pointwise_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _features_features_0_pointwise_Conv_output_0_layer, 28,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_features_features_0_pointwise_Conv_output_0_chain,
  NULL, &_features_features_0_relu_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_stem_stem_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_features_features_0_depthwise_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_features_features_0_depthwise_Conv_output_0_weights, &_features_features_0_depthwise_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _features_features_0_depthwise_Conv_output_0_layer, 27,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_features_features_0_depthwise_Conv_output_0_chain,
  NULL, &_features_features_0_pointwise_Conv_output_0_layer, AI_STATIC, 
  .groups = 16, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _stem_stem_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _stem_stem_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _stem_stem_2_Clip_output_0_nl_params_data, _stem_stem_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _stem_stem_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_stem_stem_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_stem_stem_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _stem_stem_2_Clip_output_0_layer, 26,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_stem_stem_2_Clip_output_0_chain,
  NULL, &_features_features_0_depthwise_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_stem_stem_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_Reshape_output_0_to_chfirst_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_stem_stem_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_stem_stem_0_Conv_output_0_weights, &_stem_stem_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_stem_stem_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _stem_stem_0_Conv_output_0_layer, 23,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_stem_stem_0_Conv_output_0_chain,
  NULL, &_stem_stem_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _Reshape_output_0_to_chfirst_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_Reshape_output_0_to_chfirst_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _Reshape_output_0_to_chfirst_layer, 22,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &_Reshape_output_0_to_chfirst_chain,
  NULL, &_stem_stem_0_Conv_output_0_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 50056, 1, 1),
    50056, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 239408, 1, 1),
    239408, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &output_output),
  &_Reshape_output_0_to_chfirst_layer, 0x9a4a2a84, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 50056, 1, 1),
      50056, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 239408, 1, 1),
      239408, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &output_output),
  &_Reshape_output_0_to_chfirst_layer, 0x9a4a2a84, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_output_array.data = AI_PTR(g_network_activations_map[0] + 135476);
    input_output_array.data_start = AI_PTR(g_network_activations_map[0] + 135476);
    _Reshape_output_0_to_chfirst_output_array.data = AI_PTR(g_network_activations_map[0] + 187424);
    _Reshape_output_0_to_chfirst_output_array.data_start = AI_PTR(g_network_activations_map[0] + 187424);
    _stem_stem_0_Conv_output_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 239372);
    _stem_stem_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 239372);
    _stem_stem_0_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 5440);
    _stem_stem_0_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 5440);
    _stem_stem_2_Clip_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 5440);
    _stem_stem_2_Clip_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 5440);
    _features_features_0_depthwise_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    _features_features_0_depthwise_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    _features_features_0_pointwise_Conv_output_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 53760);
    _features_features_0_pointwise_Conv_output_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 53760);
    _features_features_0_pointwise_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 53824);
    _features_features_0_pointwise_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 53824);
    _features_features_0_relu_Clip_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 53824);
    _features_features_0_relu_Clip_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 53824);
    _features_features_2_depthwise_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    _features_features_2_depthwise_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    _features_features_2_pointwise_Conv_output_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 26880);
    _features_features_2_pointwise_Conv_output_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 26880);
    _features_features_2_pointwise_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 27008);
    _features_features_2_pointwise_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 27008);
    _features_features_2_relu_Clip_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 80768);
    _features_features_2_relu_Clip_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 80768);
    _features_features_4_depthwise_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    _features_features_4_depthwise_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    _features_features_4_pointwise_Conv_output_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 16128);
    _features_features_4_pointwise_Conv_output_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 16128);
    _features_features_4_pointwise_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 16384);
    _features_features_4_pointwise_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 16384);
    _features_features_4_relu_Clip_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 48640);
    _features_features_4_relu_Clip_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 48640);
    _pool_GlobalAveragePool_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    _pool_GlobalAveragePool_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    output_output_array.data = AI_PTR(g_network_activations_map[0] + 512);
    output_output_array.data_start = AI_PTR(g_network_activations_map[0] + 512);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    _stem_stem_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _stem_stem_0_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 0);
    _stem_stem_0_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    _stem_stem_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _stem_stem_0_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 576);
    _stem_stem_0_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 576);
    _features_features_0_depthwise_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _features_features_0_depthwise_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 640);
    _features_features_0_depthwise_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 640);
    _features_features_0_depthwise_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _features_features_0_depthwise_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 1216);
    _features_features_0_depthwise_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1216);
    _features_features_0_pointwise_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _features_features_0_pointwise_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 1280);
    _features_features_0_pointwise_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 1280);
    _features_features_0_pointwise_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _features_features_0_pointwise_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 3328);
    _features_features_0_pointwise_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 3328);
    _features_features_2_depthwise_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _features_features_2_depthwise_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 3456);
    _features_features_2_depthwise_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 3456);
    _features_features_2_depthwise_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _features_features_2_depthwise_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 4608);
    _features_features_2_depthwise_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 4608);
    _features_features_2_pointwise_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _features_features_2_pointwise_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 4736);
    _features_features_2_pointwise_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 4736);
    _features_features_2_pointwise_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _features_features_2_pointwise_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 12928);
    _features_features_2_pointwise_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 12928);
    _features_features_4_depthwise_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _features_features_4_depthwise_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 13184);
    _features_features_4_depthwise_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 13184);
    _features_features_4_depthwise_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _features_features_4_depthwise_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 15488);
    _features_features_4_depthwise_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 15488);
    _features_features_4_pointwise_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _features_features_4_pointwise_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 15744);
    _features_features_4_pointwise_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 15744);
    _features_features_4_pointwise_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _features_features_4_pointwise_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 48512);
    _features_features_4_pointwise_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 48512);
    output_weights_array.format |= AI_FMT_FLAG_CONST;
    output_weights_array.data = AI_PTR(g_network_weights_map[0] + 49024);
    output_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 49024);
    output_bias_array.format |= AI_FMT_FLAG_CONST;
    output_bias_array.data = AI_PTR(g_network_weights_map[0] + 50048);
    output_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 50048);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 2293759,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x9a4a2a84,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 2293759,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x9a4a2a84,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_network_data_params_get(&params) != true) {
    err = ai_network_get_error(*network);
    return err;
  }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_network_init(*network, &params) != true) {
    err = ai_network_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

