#include "cpu.h"

static lv_obj_t *cpu_obj;
static lv_obj_t * chart1;
static lv_obj_t * chart2;
extern lv_obj_t *base_obj;
static lv_chart_series_t * ser1;
static lv_chart_series_t * ser2;
static void set_angle(void * obj, int32_t v)
{
    lv_arc_set_angles(obj, v, 360);
}
static void draw_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    /*Add the faded area before the lines are drawn*/
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    if(dsc->part == LV_PART_ITEMS) {
        if(!dsc->p1 || !dsc->p2) return;

        /*Add a line mask that keeps the area below the line*/
        lv_draw_mask_line_param_t line_mask_param;
        lv_draw_mask_line_points_init(&line_mask_param, dsc->p1->x, dsc->p1->y, dsc->p2->x, dsc->p2->y,
                                      LV_DRAW_MASK_LINE_SIDE_BOTTOM);
        int16_t line_mask_id = lv_draw_mask_add(&line_mask_param, NULL);

        /*Add a fade effect: transparent bottom covering top*/
        lv_coord_t h = lv_obj_get_height(obj);
        lv_draw_mask_fade_param_t fade_mask_param;
        lv_draw_mask_fade_init(&fade_mask_param, &obj->coords, LV_OPA_COVER, obj->coords.y1 + h / 8, LV_OPA_TRANSP,
                               obj->coords.y2);
        int16_t fade_mask_id = lv_draw_mask_add(&fade_mask_param, NULL);

        /*Draw a rectangle that will be affected by the mask*/
        lv_draw_rect_dsc_t draw_rect_dsc;
        lv_draw_rect_dsc_init(&draw_rect_dsc);
        draw_rect_dsc.bg_opa = LV_OPA_60;
        draw_rect_dsc.bg_color = dsc->line_dsc->color;

        lv_area_t a;
        a.x1 = dsc->p1->x;
        a.x2 = dsc->p2->x - 1;
        a.y1 = LV_MIN(dsc->p1->y, dsc->p2->y);
        a.y2 = obj->coords.y2;
        lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

        /*Remove the masks*/
        lv_draw_mask_free_param(&line_mask_param);
        lv_draw_mask_free_param(&fade_mask_param);
        lv_draw_mask_remove_id(line_mask_id);
        lv_draw_mask_remove_id(fade_mask_id);
    }
    /*Hook the division lines too*/
    else if(dsc->part == LV_PART_MAIN) {
        if(dsc->line_dsc == NULL || dsc->p1 == NULL || dsc->p2 == NULL) return;

        /*Vertical line*/
        if(dsc->p1->x == dsc->p2->x) {

        }
        /*Horizontal line*/
        else {

            dsc->line_dsc->color  = lv_palette_lighten(LV_PALETTE_GREY, 1);
            dsc->line_dsc->opa = LV_OPA_20;
        }
    }
}
static void add_data(lv_timer_t * arg)
{
    lv_chart_set_next_value(chart1, ser1, lv_rand(0, 100));
    lv_chart_set_next_value(chart2, ser2, lv_rand(0, 100));
}
/**
 * Add a faded area effect to the line chart and make some division lines ticker
 */

void char_show(void)
{
    static lv_obj_t *cpu_obj;
    cpu_obj = lv_obj_create(base_obj);
    lv_obj_set_size(cpu_obj, 140, 95);
    lv_obj_set_style_bg_color(cpu_obj, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_border_opa(cpu_obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_align(cpu_obj, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(cpu_obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *text = lv_label_create(base_obj);
    lv_label_set_text(text, "MEM");
    lv_obj_align(text, LV_ALIGN_TOP_LEFT,5,3);
    lv_obj_set_style_text_font(text, &lv_font_montserrat_10, LV_PART_MAIN);

    static lv_style_t font_style;
    lv_style_init(&font_style);
    lv_style_set_text_color(&font_style, lv_color_white());
    lv_obj_add_style(text, &font_style, LV_PART_MAIN);

    chart1 = lv_chart_create(base_obj);

    lv_obj_set_size(chart1, 130, 80);
    lv_obj_align(chart1, LV_ALIGN_TOP_LEFT,5,15);
    lv_obj_set_style_bg_color(chart1, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_border_opa(chart1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_opa(chart1, LV_OPA_COVER, LV_PART_MAIN);
    lv_chart_set_type(chart1, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    lv_chart_set_div_line_count(chart1, 6, 0);
    lv_chart_set_point_count(chart1, 60);

    lv_obj_add_event_cb(chart1, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
    lv_chart_set_update_mode(chart1, LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_set_style_size(chart1, 0, LV_PART_INDICATOR);
    /*Add two data series*/
    ser1 = lv_chart_add_series(chart1, lv_palette_main(LV_PALETTE_PINK), LV_CHART_AXIS_PRIMARY_Y);
    lv_timer_create(add_data,1000, NULL);


}

void char_show_2(void)
{
    static lv_obj_t *cpu_obj;
    cpu_obj = lv_obj_create(base_obj);
    lv_obj_set_size(cpu_obj, 140, 95);
    lv_obj_set_style_bg_color(cpu_obj, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_border_opa(cpu_obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_align(cpu_obj, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(cpu_obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *text = lv_label_create(base_obj);
    lv_label_set_text(text, "CPU");
    lv_obj_align(text, LV_ALIGN_BOTTOM_LEFT,5,-80);
    lv_obj_set_style_text_font(text, &lv_font_montserrat_10, LV_PART_MAIN);

    static lv_style_t font_style;
    lv_style_init(&font_style);
    lv_style_set_text_color(&font_style, lv_color_white());
    lv_obj_add_style(text, &font_style, LV_PART_MAIN);

    chart2 = lv_chart_create(base_obj);

    lv_obj_set_size(chart2, 130, 80);
    lv_obj_align(chart2, LV_ALIGN_BOTTOM_LEFT,5,0);
    lv_obj_set_style_bg_color(chart2, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_border_opa(chart2, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_opa(chart2, LV_OPA_COVER, LV_PART_MAIN);
    lv_chart_set_type(chart2, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    lv_chart_set_div_line_count(chart2, 6, 0);
    lv_chart_set_point_count(chart2, 60);

    lv_obj_add_event_cb(chart2, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
    lv_chart_set_update_mode(chart2, LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_set_style_size(chart2, 0, LV_PART_INDICATOR);
    /*Add two data series*/
    ser2 = lv_chart_add_series(chart2, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_timer_create(add_data,1000, NULL);
    // 

    

}
/**
 * Create an arc which acts as a loader.
 */
void lv_example_arc_22(void)
{
    /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(cpu_obj);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_angles(arc, 0, 360);
    lv_arc_set_bg_angles(arc, 0, 360);

    static lv_style_t fg_style,bg_style;
    lv_style_init(&fg_style);
    lv_style_init(&bg_style);

    lv_obj_set_size(arc, 60, 60);
    lv_style_set_arc_color(&fg_style, lv_color_white());
    lv_style_set_arc_rounded(&fg_style, false);
    lv_style_set_arc_width(&fg_style, 6);

    lv_style_set_arc_color(&bg_style, lv_color_hex(0xE91E63));
    lv_style_set_arc_rounded(&bg_style, false);
    lv_style_set_arc_width(&bg_style, 6);

    lv_obj_add_style(arc, &fg_style, LV_PART_MAIN);
    lv_obj_add_style(arc, &bg_style, LV_PART_INDICATOR);

    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_center(arc);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, arc);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_values(&a, 360, 0);
    lv_anim_start(&a);

}

void obj_init()
{
    cpu_obj = lv_obj_create(base_obj);
    lv_obj_set_size(cpu_obj, 80, 80);
    lv_obj_align(cpu_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_clear_flag(cpu_obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_opa(cpu_obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(cpu_obj, lv_color_hex(0x222222), LV_PART_MAIN);
}
void cpu_show()
{
    // obj_init();
    char_show();
    char_show_2();
    // lv_example_arc_22();
    return;
}
