

struct AI
{
    const static unsigned short LAYER_COUNT = 3;
    const static unsigned short LAYER_DEPTH = 4;
    const static unsigned short INPUT_COUNT = 4;
    bool training = false;
    unsigned char topology[3]{
	3, 4, 1
    };
    unsigned int data_points = 3;
    unsigned char traindata[3][4]{
        {255,0,0,255},
        {0,255,0,32},
        {0,0,255,8}
    };

    unsigned char weights[3][4][4]{
    	{{255, 0, 0, 0}, {255, 0, 0, 0}, {255, 0, 0, 0}, {0, 0, 0, 0}},
    	{{255, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    	{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    };

    unsigned char tmp[4]{0, 0, 0, 0};
    unsigned char curr[4]{0, 0, 0, 0};
    unsigned char* run(unsigned char* input);
    void train();
};
