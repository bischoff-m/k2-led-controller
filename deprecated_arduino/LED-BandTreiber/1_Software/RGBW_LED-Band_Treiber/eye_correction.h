// Lookup Table for Eye Correction LED-PWM

const unsigned char eye_correction[256] = {
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
    3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6,
    6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 9,
    9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12,
   13, 13, 13, 13, 14, 14, 14, 15 ,15, 15, 16, 16, 16, 17, 17 ,17, 
   18, 18, 18, 19, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23,
   24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31,
   32, 33, 33, 34, 34, 35, 36, 36, 37, 38, 38, 39, 40, 40, 41, 42,
   42, 43, 44, 45, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55,
   56, 57, 57, 58, 59, 60, 61, 62, 64, 65, 66, 67, 68, 69, 70, 71,
   73, 74, 75, 76, 77, 79, 80, 81, 83, 84, 85, 87, 88, 90, 91 ,93,
   94,96, 97, 99, 100, 102, 104, 105, 107, 109, 111, 112, 114, 116, 118, 120,
  122, 124, 126, 128, 130, 132, 134, 136, 138, 141, 143, 145, 147, 150, 152, 155,
  157, 160, 162, 165, 167, 170, 173, 176, 178, 181, 184, 187, 190, 193, 196, 199,
  202, 206, 209, 212, 215, 219, 222, 226, 229, 233, 237 ,240, 244, 248, 252, 255
  };

