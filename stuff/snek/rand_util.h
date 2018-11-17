//requires stdlib.h and time.h

float f_rand(float low, float high){
	if(low > high){
		std::cerr << "range invalid" << std::endl;
		return 0.0f;
	}
	high -= low;
	float random = (float)rand()/(float)RAND_MAX;
	return low + (random * high);
}

float f_rand(float high = 100){
	float random = (float)rand()/(float)RAND_MAX;
	return random * high;
}

int i_rand(int low, int high){
	if(low > high){
		std::cerr << "range invalid" << std::endl;
		return 0;
	}
	float random = (float)rand()/(float)RAND_MAX;
	high -= low;
	return low + floor(random * (float)high);
}

int i_rand(int high = 100){
	float random = (float)rand()/(float)RAND_MAX;
	return floor(random * (float)high);
}

