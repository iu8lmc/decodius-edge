// orb.c — "hello orb": sfera pulsante che reagisce a stato e livello audio.
#include "ui_lvgl.h"
#include "lvgl.h"

static lv_obj_t *s_orb = NULL;
static lv_anim_t s_pulse;

// Colori di stato (idle/listen/think/speak)
static uint32_t state_color(int s)
{
    switch (s) {
        case 1: return 0x2de2ff;  // listen
        case 2: return 0xffb02e;  // think
        case 3: return 0x3dffa0;  // speak
        default: return 0x36b6e0; // idle
    }
}

static void pulse_cb(void *obj, int32_t v)
{
    // v: 0..100 -> raggio/opacità del "respiro"
    lv_obj_set_style_transform_scale((lv_obj_t *)obj, 256 + v, 0); // 1.0..~1.4x
}

void orb_create(void *parent)
{
    s_orb = lv_obj_create((lv_obj_t *)parent);
    lv_obj_set_size(s_orb, 180, 180);
    lv_obj_align(s_orb, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_radius(s_orb, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(s_orb, lv_color_hex(state_color(0)), 0);
    lv_obj_set_style_bg_opa(s_orb, LV_OPA_80, 0);
    lv_obj_set_style_border_width(s_orb, 0, 0);
    lv_obj_clear_flag(s_orb, LV_OBJ_FLAG_SCROLLABLE);

    // Respiro continuo
    lv_anim_init(&s_pulse);
    lv_anim_set_var(&s_pulse, s_orb);
    lv_anim_set_exec_cb(&s_pulse, pulse_cb);
    lv_anim_set_values(&s_pulse, 0, 100);
    lv_anim_set_time(&s_pulse, 1700);
    lv_anim_set_playback_time(&s_pulse, 1700);
    lv_anim_set_repeat_count(&s_pulse, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&s_pulse);
}

void orb_set_state(int state)
{
    if (s_orb) lv_obj_set_style_bg_color(s_orb, lv_color_hex(state_color(state)), 0);
}

void orb_set_level(float rms)
{
    // TODO: modulare scala/glow dell'orb con il livello audio reale (RMS dall'I2S).
    (void)rms;
}
