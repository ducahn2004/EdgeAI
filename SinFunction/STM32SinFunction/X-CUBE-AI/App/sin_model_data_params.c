/**
  ******************************************************************************
  * @file    sin_model_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-08-03T18:52:44+0700
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "sin_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_sin_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_sin_model_weights_array_u64[49] = {
  0x3ef6b15f3f559166U, 0x3e5be38fbef843a5U, 0xbfa5ceb4be419898U, 0xbe928a233f58c689U,
  0xbf6b5f62bd4e5b83U, 0xbe937a363f598dfeU, 0x3f25e85a3f3eb3fdU, 0x3fc543d73ea9ee35U,
  0x3d4adcafbee06e17U, 0x3f754f0e3d577cc5U, 0xbec33db8bf64f297U, 0xbf878b833e80b031U,
  0xbf8c5ae0beff2a11U, 0xbca90a773f439b2aU, 0x3eeba2313dc31f3eU, 0x3f14d38fbe8f8b3bU,
  0x3e60b4d53f217d90U, 0x3e4b360b3eae2094U, 0xbeea75acbed68f98U, 0xbe66bc0bbe98ed3fU,
  0x3efddb54bf26be80U, 0x3f9fa06dbe91413eU, 0x3ef694eabfbe2be5U, 0xbfcdfccb3d1d975aU,
  0x3f124be83de8ec69U, 0x3ea9a0e4be4a3bbbU, 0x3dc1ceeb3eed4b4eU, 0xbedbd527befe3f89U,
  0x3edae48bbd16a077U, 0x3e9b84f3bea209baU, 0x3f0add383f753a02U, 0x3f53e98c3d6b6709U,
  0x3f164a6dbed351b9U, 0xbe967cb4bf1a5913U, 0xbe8f1b8c3f6e4315U, 0x3f3a040fbec5427bU,
  0xbf7baf33bdcc8e89U, 0xbf8284473f1a7ceaU, 0xbf248e7a3f0e0c7aU, 0x3f45e3913f172ac1U,
  0x3ecb6df83d6edf92U, 0xbde38537bd9e3586U, 0xbda7647bbd3f94a9U, 0xbc8ff9f2bcc23b7eU,
  0xbeefa774bdc3ead1U, 0xbf0fd7473ef7a0a5U, 0xbfa8d866bf60ef5eU, 0x3f530b4cc00802e5U,
  0x3e1682b1U,
};


ai_handle g_sin_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_sin_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

