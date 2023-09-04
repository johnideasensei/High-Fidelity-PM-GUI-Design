/**
 * @file lv_port_disp.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "../../lvgl.h"
#include "main.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/

static LV_ATTRIBUTE_LARGE_RAM_ARRAY lv_color_t lcdbuf_gui[LV_HOR_RES_MAX * LV_VER_RES_MAX];
static LV_ATTRIBUTE_LARGE_RAM_ARRAY lv_color_t lcdbuf_show[LV_HOR_RES_MAX * LV_VER_RES_MAX];
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[HOR_RES * 10];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, HOR_RES * 10);   /*Initialize the display buffer*/

    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[LV_HOR_RES_MAX * 10];                        /*A buffer for 10 rows*/
    static lv_color_t buf_2_2[LV_HOR_RES_MAX * 10];                        /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/

    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[LV_HOR_RES_MAX * LV_VER_RES_MAX]__attribute__((section(".SDRAM1")));            /*A screen sized buffer*/
    // static lv_color_t buf_3_2[LV_HOR_RES_MAX * LV_VER_RES_MAX]__attribute__((section(".SDRAM1")));            /*An other screen sized buffer*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, LV_HOR_RES_MAX * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    /*Required for Example 3)*/
    // disp_drv.full_refresh = 1;

    /*Rotate*/
    // disp_drv.sw_rotate = 1;
    // disp_drv.rotated = LV_DISP_ROT_90;
    
    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    DMA2D_InitStructure DMA2D_initStruct;
    DMA2D_initStruct.Interval = CyclesPerUs;
    DMA2D_initStruct.IntEOTEn = 1;
    DMA2D_Init(&DMA2D_initStruct);
    DMA2D->L[DMA2D_LAYER_FG].OR  = 0;
    #if (LV_COLOR_DEPTH == 16)
    DMA2D->L[DMA2D_LAYER_FG].PFCCR = (DMA2D_FMT_RGB565 << DMA2D_PFCCR_CFMT_Pos);
    #endif
    #if (LV_COLOR_DEPTH == 32)
    DMA2D->L[DMA2D_LAYER_FG].PFCCR = (DMA2D_FMT_ARGB888 << DMA2D_PFCCR_CFMT_Pos);
    #endif
    rgb_init();
    LCD->L[0].ADDR = (uint32_t)lcdbuf_gui;
    // LCD->L[0].ADDR = (uint32_t)lcdbuf_show;
    LCD_Start(LCD);
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    DMA2D->L[DMA2D_LAYER_FG].MAR = (uint32_t)color_p;

    DMA2D->L[DMA2D_LAYER_OUT].MAR = (uint32_t)&lcdbuf_gui[LV_HOR_RES_MAX * area->y1 + area->x1];
    DMA2D->L[DMA2D_LAYER_OUT].OR  = LV_HOR_RES_MAX - (area->x2 - area->x1 + 1);

    DMA2D->NLR = ((area->y2 - area->y1) << DMA2D_NLR_NLINE_Pos) |
                 ((area->x2 - area->x1) << DMA2D_NLR_NPIXEL_Pos);

    DMA2D->CR &= ~DMA2D_CR_MODE_Msk;
    DMA2D->CR |= (0 << DMA2D_CR_MODE_Pos) |
                 (1 << DMA2D_CR_START_Pos);
}

void DMA2D_Handler(void)
{
    DMA2D_INTClr();
    lv_disp_flush_ready(&disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
