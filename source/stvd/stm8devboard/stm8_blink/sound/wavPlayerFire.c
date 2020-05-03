#include "sound.h"


const unsigned char _wavPlayerFire[] =
{
    128, 167, 167, 183, 183, 190, 190, 194, // 0-7
    194, 194, 194, 195, 195, 196, 196, 195, // 8-15
    195, 193, 193, 189, 189, 183, 183, 174, // 16-23
    174, 162, 162, 128, 128,  93,  93,  81, // 24-31
     81,  72,  72,  66,  66,  62,  62,  60, // 32-39
     60,  59,  59,  60,  60,  63,  63,  67, // 40-47
     67,  74,  74,  83,  83,  97,  97, 145, // 48-55
    145, 165, 165, 176, 176, 184, 184, 190, // 56-63
    190, 193, 193, 195, 195, 195, 195, 194, // 64-71
    194, 191, 191, 186, 186, 179, 179, 170, // 72-79
    170, 154, 154, 104, 104,  87,  87,  77, // 80-87
     77,  70,  70,  65,  65,  61,  61,  60, // 88-95
     60,  60,  60,  62,  62,  65,  65,  70, // 96-103
     70,  78,  78,  88,  88, 106, 106, 156, // 104-111
    156, 170, 170, 180, 180, 187, 187, 191, // 112-119
    191, 194, 194, 195, 195, 195, 195, 193, // 120-127
    193, 189, 189, 183, 183, 175, 175, 164, // 128-135
    164, 141, 141,  95,  95,  82,  82,  74, // 136-143
     74,  67,  67,  63,  63,  61,  61,  60, // 144-151
     60,  61,  61,  63,  63,  67,  67,  73, // 152-159
     73,  82,  82,  95,  95, 139, 139, 163, // 160-167
    163, 175, 175, 183, 183, 189, 189, 192, // 168-175
    192, 194, 194, 195, 195, 194, 194, 191, // 176-183
    191, 187, 187, 180, 180, 171, 171, 156, // 184-191
    156, 107, 107,  89,  89,  78,  78,  71, // 192-199
     71,  66,  66,  62,  62,  60,  60,  60, // 200-207
     60,  62,  62,  65,  65,  70,  70,  77, // 208-215
     77,  87,  87, 103, 103, 153, 153, 169, // 216-223
    169, 179, 179, 185, 185, 190, 190, 193, // 224-231
    193, 195, 195, 194, 194, 193, 193, 189, // 232-239
    189, 184, 184, 176, 176, 165, 165, 146, // 240-247
    146,  98,  98,  84,  84,  75,  75,  68, // 248-255
     68,  64,  64,  61,  61,  60,  60,  61, // 256-263
     61,  63,  63,  67,  67,  73,  73,  81, // 264-271
     81,  93,  93, 120, 120, 161, 161, 173, // 272-279
    173, 182, 182, 188, 188, 192, 192, 194, // 280-287
    194, 194, 194, 194, 194, 191, 191, 187, // 288-295
    187, 181, 181, 172, 172, 158, 158, 112, // 296-303
    112,  91,  91,  80,  80,  72,  72,  66, // 304-311
     66,  63,  63,  61,  61,  61,  61,  62, // 312-319
     62,  65,  65,  70,  70,  76,  76,  86, // 320-327
     86, 101, 101, 150, 150, 167, 167, 177, // 328-335
    177, 184, 184, 189, 189, 193, 193, 194, // 336-343
    194, 194, 194, 192, 192, 189, 189, 184, // 344-351
    184, 177, 177, 167, 167, 149, 149, 100, // 352-359
    100,  86,  86,  76,  76,  69,  69,  65, // 360-367
     65,  62,  62,  61,  61,  61,  61,  63, // 368-375
     63,  67,  67,  72,  72,  80,  80,  91, // 376-383
     91, 113, 113, 159, 159, 172, 172, 180, // 384-391
    180, 187, 187, 191, 191, 193, 193, 194, // 392-399
    194, 193, 193, 191, 191, 187, 187, 181, // 400-407
    181, 173, 173, 160, 160, 118, 118,  93, // 408-415
     93,  81,  81,  73,  73,  67,  67,  64, // 416-423
     64,  62,  62,  61,  61,  62,  62,  65, // 424-431
     65,  69,  69,  76,  76,  85,  85,  98, // 432-439
     98, 146, 146, 165, 165, 176, 176, 183, // 440-447
    183, 188, 188, 192, 192, 194, 194, 194, // 448-455
    194, 192, 192, 189, 189, 184, 184, 178, // 456-463
    178, 168, 168, 152, 152, 103, 103,  87, // 464-471
     87,  77,  77,  71,  71,  66,  66,  63, // 472-479
     63,  61,  61,  62,  62,  63,  63,  67, // 480-487
     67,  72,  72,  79,  79,  90,  90, 109, // 488-495
    109, 156, 156, 170, 170, 179, 179, 186, // 496-503
    186, 190, 190, 193, 193, 194, 194, 193, // 504-511
    193, 191, 191, 187, 187, 182, 182, 174, // 512-519
    174, 162, 162, 137, 137,  95,  95,  83, // 520-527
     83,  74,  74,  68,  68,  65,  65,  62, // 528-535
     62,  62,  62,  62,  62,  65,  65,  69, // 536-543
     69,  75,  75,  84,  84,  97,  97, 142, // 544-551
    142, 163, 163, 174, 174, 182, 182, 187, // 552-559
    187, 191, 191, 193, 193, 193, 193, 192, // 560-567
    192, 189, 189, 185, 185, 178, 178, 169, // 568-575
    169, 155, 155, 106, 106,  89,  89,  79, // 576-583
     79,  72,  72,  67,  67,  64,  64,  62, // 584-591
     62,  62,  62,  64,  64,  67,  67,  72, // 592-599
     72,  79,  79,  89,  89, 105, 105, 154, // 600-607
    154, 168, 168, 178, 178, 184, 184, 189, // 608-615
    189, 192, 192, 193, 193, 193, 193, 191, // 616-623
    191, 187, 187, 182, 182, 174, 174, 163, // 624-631
    163, 143, 143,  97,  97,  84,  84,  76, // 632-639
     76,  70,  70,  65,  65,  63,  63,  62, // 640-647
     62,  63,  63,  65,  65,  69,  69,  75, // 648-655
     75,  83,  83,  95,  95, 134, 134, 161, // 656-663
    161, 173, 173, 181, 181, 186, 186, 190, // 664-671
    190, 192, 192, 193, 193, 192, 192, 189, // 672-679
    189, 185, 185, 179, 179, 170, 170, 157, // 680-687
    157, 110, 110,  91,  91,  80,  80,  73, // 688-695
     73,  68,  68,  64,  64,  63,  63,  62, // 696-703
     62,  64,  64,  67,  67,  71,  71,  78, // 704-711
     78,  88,  88, 103, 103, 151, 151, 167, // 712-719
    167, 176, 176, 183, 183, 188, 188, 191, // 720-727
    191, 192, 192, 192, 192, 191, 191, 187, // 728-735
    187, 182, 182, 175, 175, 165, 165, 147, // 736-743
    147, 100, 100,  86,  86,  77,  77,  71, // 744-751
     71,  66,  66,  64,  64,  63,  63,  63, // 752-759
     63,  65,  65,  69,  69,  74,  74,  82, // 760-767
     82,  93,  93, 116, 116, 159, 159, 171, // 768-775
    171, 180, 180, 185, 185, 189, 189, 192, // 776-783
    192, 192, 192, 192, 192, 189, 189, 185, // 784-791
    185, 179, 179, 171, 171, 158, 158, 115, // 792-799
    115,  93,  93,  82,  82,  74,  74,  69, // 800-807
     69,  65,  65,  63,  63,  63,  63,  64, // 808-815
     64,  67,  67,  71,  71,  77,  77,  86, // 816-823
     86, 100, 100, 148, 148, 165, 165, 175, // 824-831
    175, 182, 182, 187, 187, 190, 190, 192, // 832-839
    192, 192, 192, 191, 191, 187, 187, 183, // 840-847
    183, 176, 176, 166, 166, 150, 150, 102, // 848-855
    102,  88,  88,  78,  78,  72,  72,  67, // 856-863
     67,  64,  64,  63,  63,  63,  63,  65, // 864-871
     65,  69,  69,  74,  74,  81,  81,  92, // 872-879
     92, 111, 111, 157, 157, 170, 170, 178, // 880-887
    178, 184, 184, 189, 189, 191, 191, 192, // 888-895
    192, 191, 191, 189, 189, 185, 185, 180, // 896-903
    180, 172, 172, 160, 160, 128, 128,  95, // 904-911
     95,  83,  83,  75,  75,  70,  70,  66, // 912-919
     66,  64,  64,  63,  63,  64,  64,  67, // 920-927
     67,  71,  71,  77,  77,  85,  85,  98, // 928-935
     98, 144, 144, 163, 163, 174, 174, 181, // 936-943
    181, 186, 186, 190, 190, 191, 191, 192, // 944-951
    192, 190, 190, 187, 187, 183, 183, 176, // 952-959
    176, 167, 167, 153, 153, 105, 105,  89, // 960-967
     89,  80,  80,  73,  73,  68,  68,  65, // 968-975
     65,  64,  64,  64,  64,  65,  65,  68, // 976-983
     68,  73,  73,  80,  80,  90,  90, 107, // 984-991
    107, 154, 154, 168, 168, 177, 177, 183, // 992-999
};

const SoundData wavSoundPlayerFire = 
{
    1000, 									//unsigned int length
    (const unsigned char*)_wavPlayerFire,	//pointer to sound data
};
