#include <cstdio>
#include <functional>
#include <string>
#include <vector>

#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

#define USE_MODEL_DLL_EXPORTS
#include "use_model_dll.h"

using tensorflow::string;
using tensorflow::int32;

struct tfContext
{
	tensorflow::Session* session;
	tensorflow::Scope* root;
};

tfContext* tfLoadModel(const char* modelPath, const char* checkpointName, const char* filenameTensorName, const char* restoreOpName)
{
	using namespace tensorflow;

	tfContext* ctx = new tfContext;
	tensorflow::SessionOptions options;
	ctx->session = NewSession(options);
	ctx->root = new tensorflow::Scope(tensorflow::Scope::NewRootScope());

	// First create the graph specified in the graph def file
	GraphDef graph_def;
	auto load_graph_status = ReadTextProto(tensorflow::Env::Default(), modelPath,
		&graph_def);
	if (!load_graph_status.ok())
	{
		std::cout << load_graph_status.ToString() << std::endl;
	}
	auto session_status = ctx->session->Create(graph_def);
	if (!session_status.ok())
	{
		std::cout << session_status.ToString() << std::endl;
	}

	// Now restore variable values using the Saver ops.
	std::vector<std::pair<std::string, Tensor>> input;
	Tensor filename_tensor = Tensor(DT_STRING, TensorShape({ 1 }));
	filename_tensor.vec<string>()(0) = checkpointName;
	input.emplace_back(filenameTensorName, filename_tensor);
	TF_CHECK_OK(ctx->session->Run(input, {}, { restoreOpName }, {}));

	return ctx;
}

void tfRunModel1D(tfContext* ctx, float* inputArray, int inputLength, int inputWidth, float* outputArray, int outputLength)
{
	std::vector<std::pair<std::string, tensorflow::Tensor>> input;
	std::vector<tensorflow::Tensor> output;

	auto input_tensor = tensorflow::Tensor(tensorflow::DT_FLOAT,
		tensorflow::TensorShape({ inputWidth, inputLength }));
	auto flatInput = input_tensor.flat<float>();

	int element = 0;
	for (int i = 0; i < inputWidth; i++) //TODO here test looping through inner or outer dimension first
	{
		for (int j = 0; j < inputLength; j++)
		{
			flatInput(element++) = inputArray[i*inputLength + j];
		}
	}

	input.emplace_back("input", input_tensor);

	TF_CHECK_OK(ctx->session->Run(input, { "output" }, {}, &output));

	auto flatOutput = output[0].flat<float>();

	for (int i = 0; i < outputLength; i++)
	{
		outputArray[i] = flatOutput(i);
	}
}

//void tfRunModel1D(tfContext* ctx, float* inputArray, int inputLength, float* outputArray, int outputLength)
//{
//	std::vector<std::pair<std::string, tensorflow::Tensor>> input;
//	std::vector<tensorflow::Tensor> output;
//
//	auto input_tensor = tensorflow::Tensor(tensorflow::DT_FLOAT,
//		tensorflow::TensorShape({ 1, inputLength }));
//	auto flatInput = input_tensor.flat<float>();
//	for (int i = 0; i < inputLength; i++)
//	{
//		flatInput(i) = inputArray[i];
//	}
//
//	input.emplace_back("input", input_tensor);
//
//	TF_CHECK_OK(ctx->session->Run(input, { "output" }, {}, &output));
//
//	auto flatOutput = output[0].flat<float>();
//
//	for (int i = 0; i < outputLength; i++)
//	{
//		outputArray[i] = flatOutput(i);
//	}
//}