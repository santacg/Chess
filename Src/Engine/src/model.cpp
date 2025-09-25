#include "../includes/model.h"
#include <algorithm>
#include <cmath>

using namespace std;

ChessNN::ChessNN(const string &model_path)
    : env(ORT_LOGGING_LEVEL_WARNING, "chess_nn"), session_options(),
      session(env, model_path.c_str(), session_options) {}

pair<vector<float>, vector<float>>
ChessNN::board_to_input(const bitset<64> *pieces_bb, const int turn) {
  vector<float> input_white, input_black;

  input_white.reserve(769);
  input_black.reserve(769);

  if (turn == 0) {
    input_white.push_back(1.0f);
    input_black.push_back(0.0f);
  } else {
    input_white.push_back(0.0f);
    input_black.push_back(1.0f);
  }

  for (int i = 0; i < 12; i += 2) {
    for (int sq = 0; sq < 64; sq++) {
      input_white.push_back((float)pieces_bb[i][sq]);
      input_black.push_back((float)pieces_bb[i][sq ^ 56]);
    }
  }

  for (int i = 1; i < 12; i += 2) {
    for (int sq = 0; sq < 64; sq++) {
      input_white.push_back((float)pieces_bb[i][sq]);
      input_black.push_back((float)pieces_bb[i][sq ^ 56]);
    }
  }
  return {input_white, input_black};
}

float ChessNN::predict(const bitset<64> *pieces_bb, const int turn) {
  auto [input_white, input_black] = board_to_input(pieces_bb, turn);

  vector<float> input(2 * 769);
  for (int i = 0; i < 769; ++i) {
    input[0 * (2 * 769) + 0 * 769 + i] = input_white[i];
    input[0 * (2 * 769) + 1 * 769 + i] = input_black[i];
  }

  vector<int64_t> input_shape = {1, 2, 769};

  Ort::MemoryInfo mem_info =
      Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

  Ort::Value input_tensor =
      Ort::Value::CreateTensor<float>(mem_info, input.data(), input.size(),
                                      input_shape.data(), input_shape.size());

  std::vector<std::string> input_names = session.GetInputNames();
  std::vector<std::string> output_names = session.GetOutputNames();

  std::vector<const char *> input_ptrs;
  std::vector<const char *> output_ptrs;
  for (const auto &s : input_names)
    input_ptrs.push_back(s.c_str());
  for (const auto &s : output_names)
    output_ptrs.push_back(s.c_str());

  Ort::RunOptions run_options;

  auto output_tensors = session.Run(run_options, input_ptrs.data(),
                                    &input_tensor, 1, output_ptrs.data(), 1);

  float *output_data = output_tensors.front().GetTensorMutableData<float>();
  float result = output_data[0];

  float epsilon = 1e-7f;
  result = clamp(result, epsilon, 1.0f - epsilon);
  float linear_result = log(result / (1.0f - result)) * 400;

  return linear_result;
}
