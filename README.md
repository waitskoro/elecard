# Сборка проекта
cd /home/waitskoro/projects/elecard
mkdir build
cd build
cmake ..
make

# Запуск
./elecard ../resources/SOCCER_352x288_30_orig_02.yuv ../resources/Couple.bmp output.yuv 352 288