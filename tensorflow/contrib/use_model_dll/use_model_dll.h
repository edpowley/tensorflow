#pragma once

#ifdef USE_MODEL_DLL_EXPORTS
#define USE_MODEL_DLL_API extern "C" __declspec(dllexport)
#else
#define USE_MODEL_DLL_API extern "C" __declspec(dllimport)
#endif

/**
An opaque struct holding a TensorFlow session and scope.
**/
struct tfContext;

/**
	Create a TensorFlow session and scope, and load the graph from checkpoint
	and graph definition files.

	modelPath				path to the graph definition file
	checkpointName			root checkpoint name. That is, if all checkpoint
							files are called "models/model_final.x", this
							should be "models/model_final".
	filenameTensorName		provided by the Python script. Probably
							something like "save/Const". Strip any ":0"
							off the end.
	restoreOpName			provided by the Python script. Probably
							something like "save/restore_all".
**/
USE_MODEL_DLL_API tfContext* tfLoadModel(const char* modelPath, const char* checkpointName, const char* filenameTensorName, const char* restoreOpName);

/**
	Run a model previously loaded by loadModel.
	The input can be a 1- or 2-dimensional array, the output is a 1-dimensional array of floats.
	Note that it is the caller's responsibility to ensure that inputLength, inputWidth and
	outputLength match the parameters of the model being used.

	ctx						a context returned by loadModel.
	inputArray				pointer to the input data.
	inputLength				in the 1D case: the number of input floats; in the 2D case: the number of columns of the input array
	inputWidth				in the 1D case: 1; in the 2D case: the number of rows of the input array
	outputArray				pointer to a buffer which will receive the output
							data.
	outputLength			how many elements should be written to outputArray.
**/
USE_MODEL_DLL_API void tfRunModel1D(tfContext* ctx, float* inputArray, int inputLength, int inputWidth, float* outputArray, int outputLength);
