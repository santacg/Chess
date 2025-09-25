#ifndef MODEL_H
#define MODEL_H

#include <bitset>
#include <onnxruntime/onnxruntime_cxx_api.h>
#include <string>
#include <utility>
#include <vector>

class ChessNN {
public:
  ChessNN(const std::string &model_path);

  std::pair<std::vector<float>, std::vector<float>>
  board_to_input(const std::bitset<64> *pieces_bb, int turn);

  float predict(const std::bitset<64> *pieces_bb, int turn);

private:
  Ort::Env env;
  Ort::SessionOptions session_options;
  Ort::Session session;
  Ort::AllocatorWithDefaultOptions allocator;
};

#endif
