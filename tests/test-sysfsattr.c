/*
 * SPDX-FileCopyrightText: 2013 Canonical Ltd.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Martin Pitt <martin.pitt@ubuntu.com>
 */

#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <glib.h>
#include <gio/gio.h>
#include <umockdev.h>

#include <gudev/gudev.h>

typedef struct {
	UMockdevTestbed *testbed;
} Fixture;

static void
fixture_setup (Fixture *f, G_GNUC_UNUSED const void *data)
{
	f->testbed = umockdev_testbed_new ();

	g_assert (umockdev_in_mock_environment ());
}

static void
fixture_teardown (Fixture *f, G_GNUC_UNUSED const void *data)
{
	g_clear_object (&f->testbed);
}

static void
test_uncached_sysfs_attr (Fixture *f, G_GNUC_UNUSED const void *data)
{
	umockdev_testbed_add_device (f->testbed, "platform", "dev1", NULL,
				     "dytc_lapmode", "1", "console", "Y\n", NULL,
				     "ID_MODEL", "KoolGadget", NULL);

	/* Check the number of items in GUdevClient */
	const gchar *subsystems[] = { "platform", NULL};
	GUdevClient *client = g_udev_client_new (subsystems);
	GUdevDevice *dev;
	g_autofree char *lapmode_path = NULL;
	g_autofree char *console_path = NULL;
	FILE *sysfsfp;

	GList *devices = g_udev_client_query_by_subsystem (client, NULL);
	g_assert_cmpint (g_list_length (devices), ==, 1);
	dev = devices->data;
	lapmode_path = g_build_filename (g_udev_device_get_sysfs_path (dev), "dytc_lapmode", NULL);
	/* First access */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	g_assert_cmpstr (g_udev_device_get_sysfs_attr (dev, "dytc_lapmode"), ==, "1");
	sysfsfp = fopen (lapmode_path, "w");
	fprintf (sysfsfp, "%s\n", "0");
	fclose (sysfsfp);
	/* This is cached */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	/* This is uncached, and updates the cache */
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "dytc_lapmode"));
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	g_assert_cmpstr (g_udev_device_get_sysfs_attr (dev, "dytc_lapmode"), ==, "0");

	/* Test N/Y and trailing linefeeds */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "console"));
	console_path = g_build_filename (g_udev_device_get_sysfs_path (dev), "console", NULL);
	sysfsfp = fopen (console_path, "w");
	fprintf (sysfsfp, "%s\n", "N");
	fclose (sysfsfp);
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "console"));
	sysfsfp = fopen (console_path, "w");
	fprintf (sysfsfp, "%s\n", "Y");
	fclose (sysfsfp);
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "console"));

	g_list_free_full (devices, g_object_unref);
}

int main(int argc, char **argv)
{
	setlocale (LC_ALL, NULL);
	g_test_init (&argc, &argv, NULL);

	g_test_add ("/gudev/uncached_sysfs_attr", Fixture, NULL,
	            fixture_setup,
	            test_uncached_sysfs_attr,
	            fixture_teardown);

	return g_test_run ();
}
