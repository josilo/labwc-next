// SPDX-License-Identifier: GPL-2.0-only
#include <math.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <cmocka.h>
#include <wlr/config.h>
#include "common/macros.h"
#include "config/libinput.h"

static void
test_curve(void **state)
{
	struct lab_accel_curve curve = {0};
	assert_true(libinput_curve_parse(0.1, "0 0.10 0.4", &curve));
	assert_int_equal(curve.nr_points, 3);
	assert_true(curve.step == 0.1);
	assert_true(curve.points[1] == 0.1);
	assert_true(libinput_curve_parse(1, " 0\t1\n2 ", &curve));
	assert_true(curve.step == 1);
	assert_int_equal(curve.nr_points, 3);
	assert_true(curve.points[2] == 2);
	/* Replacement must not retain points from the previous longer curve. */
	assert_true(libinput_curve_parse(1, "0 1", &curve));
	assert_int_equal(curve.nr_points, 2);
	assert_true(curve.points[2] == 0);
}

static void
test_max_points(void **state)
{
	char points[512] = {0};
	size_t len = 0;
	for (size_t i = 0; i < LAB_ACCEL_CURVE_MAX_POINTS; ++i) {
		len += snprintf(points + len, sizeof(points) - len, "%zu ", i);
	}
	struct lab_accel_curve curve = {0};
	assert_true(libinput_curve_parse(1, points, &curve));
	assert_int_equal(curve.nr_points, LAB_ACCEL_CURVE_MAX_POINTS);
	assert_true(curve.points[LAB_ACCEL_CURVE_MAX_POINTS - 1]
		== LAB_ACCEL_CURVE_MAX_POINTS - 1);
}

static void
test_invalid_curve(void **state)
{
	static const struct { double step; const char *points; } cases[] = {
		{0, "0 1"}, {-1, "0 1"}, {NAN, "0 1"}, {INFINITY, "0 1"},
		{1, NULL}, {1, ""}, {1, "  "}, {1, "0"}, {1, "0 -1"},
		{1, "0 nan"}, {1, "0 inf"}, {1, "0 1e999"}, {1, "0 0.100.10"},
		{1, "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 "
			"17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32"},
	};
	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		struct lab_accel_curve curve = {
			.step = 42,
			.points = {0, 3},
			.nr_points = 2,
		};
		assert_false(libinput_curve_parse(cases[i].step, cases[i].points, &curve));
		/* Parsing failures must not partially replace a prior curve. */
		assert_true(curve.step == 42);
		assert_int_equal(curve.nr_points, 2);
		assert_true(curve.points[1] == 3);
	}
}

#if WLR_HAS_LIBINPUT_BACKEND
static void
test_native_config(void **state)
{
	struct libinput_category category = {0};
	for (int i = LIBINPUT_ACCEL_TYPE_FALLBACK; i <= LIBINPUT_ACCEL_TYPE_SCROLL; ++i) {
		assert_true(libinput_curve_parse(1, "0 1",
			&category.accel_curves[i]));
	}
	struct libinput_config_accel *config = libinput_custom_accel_create(&category);
	assert_non_null(config);
	libinput_config_accel_destroy(config);
	category.accel_curve_invalid = true;
	assert_null(libinput_custom_accel_create(&category));
	category.accel_curve_invalid = false;
	category.accel_curves[LIBINPUT_ACCEL_TYPE_SCROLL].step = 0;
	assert_null(libinput_custom_accel_create(&category));
	/* An opt-in with no curves uses libinput defaults. */
	memset(&category, 0, sizeof(category));
	config = libinput_custom_accel_create(&category);
	assert_non_null(config);
	libinput_config_accel_destroy(config);
}
#endif

int
main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_curve),
		cmocka_unit_test(test_max_points),
		cmocka_unit_test(test_invalid_curve),
#if WLR_HAS_LIBINPUT_BACKEND
		cmocka_unit_test(test_native_config),
#endif
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
