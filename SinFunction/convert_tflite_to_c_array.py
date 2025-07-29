# convert_tflite_to_c_array.py

def convert_to_c_array(tflite_file, output_file):
    with open(tflite_file, 'rb') as f:
        data = f.read()

    with open(output_file, 'w') as f:
        f.write('const unsigned char sin_model_tflite[] = {\n  ')
        for i, b in enumerate(data):
            f.write(f'0x{b:02x}, ')
            if (i + 1) % 12 == 0:
                f.write('\n  ')
        f.write('\n};\n')
        f.write(f'const unsigned int sin_model_tflite_len = {len(data)};\n')

# Gọi hàm
convert_to_c_array("sin_model.tflite", "sin_model_data.cc")
