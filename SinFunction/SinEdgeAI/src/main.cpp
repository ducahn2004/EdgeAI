#include <Arduino.h>
#include "NeuralNetwork.h"

NeuralNetwork *nn;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // chờ Serial sẵn sàng
  }

  Serial.println("Initializing Neural Network...");
  nn = new NeuralNetwork();
}

void loop()
{
  // Tạo giá trị đầu vào ngẫu nhiên từ 0 đến 2π
  float x = random(0, 628) / 100.0; // từ 0.00 đến 6.28

  // Gán giá trị đầu vào cho model
  nn->getInputBuffer()[0] = x;

  // Dự đoán giá trị sin(x)
  float y_pred = nn->predict();

  // Giá trị sin thực tế
  float y_true = sin(x);

  // In kết quả ra Serial
  Serial.printf("x = %.2f | Predicted: %.4f | Actual: %.4f | Error: %.4f\n",
                x, y_pred, y_true, fabs(y_true - y_pred));

  delay(1000);
}
