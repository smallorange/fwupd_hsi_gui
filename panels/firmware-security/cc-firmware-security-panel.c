/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2021 Red Hat, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kate Hsuan <hpa@redhat.com>
 */

#include "list-box-helper.h"
#include "cc-firmware-security-panel.h"
#include "cc-firmware-security-resources.h"
#include "cc-firmware-security-dialog.h"
#include "cc-firmware-security-boot-dialog.h"
#include "cc-firmware-security-utils.h"
#include "cc-util.h"

#include <gio/gdesktopappinfo.h>
#include <glib/gi18n.h>

#define LOCATION_ENABLED "enabled"
#define APP_PERMISSIONS_TABLE "location"
#define APP_PERMISSIONS_ID "location"

struct _CcfirmwareSecurityPanel
{
  CcPanel       parent_instance;

  GtkButton     *hsi_button;
  GtkButton     *secure_boot_button;

  /* HSI button */
  GtkWidget       *hsi_grid;
  GtkWidget       *hsi_icon;
  GtkWidget       *hsi_label;
  GtkWidget       *hsi_description;

  /* Secure boot button */
  GtkWidget       *secure_boot_button_grid;
  GtkWidget       *secure_boot_icon;
  GtkWidget       *secure_boot_label;
  GtkWidget       *secure_boot_description;


  GDBusProxy   *bus_proxy;
  GDBusProxy   *properties_bus_proxy;

  GHashTable   *firmware_security_attrs;

  GHashTable   *hsi1_hash_table;
  GHashTable   *hsi2_hash_table;
  GHashTable   *hsi3_hash_table;
  GHashTable   *hsi4_hash_table;
  GHashTable   *hsi_general_hash_table;
  GHashTable   *event_hash_table;

  gint     hsi_number;
  gboolean is_secure_boot;

  GSettings    *firmware_security_settings;
  GtkSizeGroup *firmware_security_icon_size_group;
};

CC_PANEL_REGISTER (CcfirmwareSecurityPanel, cc_firmware_security_panel)


typedef enum {
	FWUPD_STATUS_UNKNOWN,
	FWUPD_STATUS_IDLE,
	FWUPD_STATUS_LOADING,
	FWUPD_STATUS_DECOMPRESSING,
	FWUPD_STATUS_DEVICE_RESTART,
	FWUPD_STATUS_DEVICE_WRITE,
	FWUPD_STATUS_DEVICE_VERIFY,
	FWUPD_STATUS_SCHEDULING,
	FWUPD_STATUS_DOWNLOADING,
	FWUPD_STATUS_DEVICE_READ,
	FWUPD_STATUS_DEVICE_ERASE,
	FWUPD_STATUS_WAITING_FOR_AUTH,
	FWUPD_STATUS_DEVICE_BUSY,
	FWUPD_STATUS_SHUTDOWN,
	/*< private >*/
	FWUPD_STATUS_LAST
} FwupdStatus;

const gchar *
fwupd_status_to_string(FwupdStatus status)
{
	if (status == FWUPD_STATUS_UNKNOWN)
		return "unknown";
	if (status == FWUPD_STATUS_IDLE)
		return "idle";
	if (status == FWUPD_STATUS_DECOMPRESSING)
		return "decompressing";
	if (status == FWUPD_STATUS_LOADING)
		return "loading";
	if (status == FWUPD_STATUS_DEVICE_RESTART)
		return "device-restart";
	if (status == FWUPD_STATUS_DEVICE_WRITE)
		return "device-write";
	if (status == FWUPD_STATUS_DEVICE_READ)
		return "device-read";
	if (status == FWUPD_STATUS_DEVICE_ERASE)
		return "device-erase";
	if (status == FWUPD_STATUS_DEVICE_VERIFY)
		return "device-verify";
	if (status == FWUPD_STATUS_DEVICE_BUSY)
		return "device-busy";
	if (status == FWUPD_STATUS_SCHEDULING)
		return "scheduling";
	if (status == FWUPD_STATUS_DOWNLOADING)
		return "downloading";
	if (status == FWUPD_STATUS_WAITING_FOR_AUTH)
		return "waiting-for-auth";
	if (status == FWUPD_STATUS_SHUTDOWN)
		return "shutdown";
	return NULL;
}

static void
parse_iter_from_key(CcfirmwareSecurityPanel *self, const gchar *key, GVariant *value)
{
	if (g_strcmp0 (key, "AppstreamId") == 0) {
    printf("Appstream: %s\n", g_variant_get_string (value, NULL));
		return;
	}
}


static void
set_secure_boot_button_view(CcfirmwareSecurityPanel *self)
{
  gint64 flags;
  gint64 *result = g_hash_table_lookup(self->hsi_general_hash_table, "org.fwupd.hsi.Uefi.SecureBoot");

  if(result == NULL)
    flags = 0;
  else
    flags = GPOINTER_TO_INT(result);

  switch(flags % 2)
  {
    case 1:
              gtk_image_set_from_icon_name(self->secure_boot_icon, "security-low-symbolic", 70);
              gtk_label_set_text(self->secure_boot_label, "Secure Boot is Active");
              gtk_label_set_text(self->secure_boot_description, "Secure boot prevents malicious software from being loaded when the device starts\n\nFor more information on secure boot, contact the manufacture support or IT support.");
              self->is_secure_boot = TRUE;
              break;
    case 2:
    default:
              gtk_image_set_from_icon_name(self->secure_boot_icon, "security-low-symbolic", 70);
              gtk_label_set_text(self->secure_boot_label, "Secure Boot is Inactive");
              gtk_label_set_text(self->secure_boot_description, "Secure boot prevents malicious software from being loaded when the device starts\n\nFor more information on secure boot, contact the manufacture support or IT support.");
              self->is_secure_boot = FALSE;
  }
}


static void
event_hash_insert(CcfirmwareSecurityPanel *self, const gchar *app_name, GVariantIter *iter)
{
  GVariant *value;
  const gchar *key;
  guint64 result = 0;

  while (g_variant_iter_next(iter, "{&sv}", &key, &value)) {
    if(!g_strcmp0(key, "HsiResult"))
    {
      result = g_variant_get_uint32(value);
      printf("Key %s flag %u\n", app_name, result);
      g_hash_table_insert(self->event_hash_table, app_name, result);
      return;
    }
  }
}


static void
event_build_string(CcfirmwareSecurityPanel *self, const gchar *app_name, const guint64 results, const guint64 timestamp)
{
  struct GDatetime *date = g_date_time_new_from_unix_local(timestamp);
  g_autofree gchar *date_string;
  guint64 *result_origin;
  date_string = g_date_time_format(date, "\%Y-\%m-\%d \%H:\%m:\%S");
  result_origin = g_hash_table_lookup(self->event_hash_table, app_name);
  printf("%s %s changed from %s to %s %d to %d\n", date_string, fu_security_attr_get_name(app_name),
         fwupd_security_attr_result_to_string(results),
         fwupd_security_attr_result_to_string(GPOINTER_TO_INT(result_origin)),
         results,
         result_origin);
  g_date_time_unref(date);
}


static void
event_on_hit(CcfirmwareSecurityPanel *self, const gchar *app_name, GVariantIter *iter)
{
  GVariant *value;
  const gchar *key;
  guint32 result = 0;
  guint64 timestamp = 0;

  while (g_variant_iter_next(iter, "{&sv}", &key, &value)) {
    if(!g_strcmp0(key, "HsiResult"))
    {
      result = g_variant_get_uint32(value);
    }else if(!g_strcmp0(key, "Created"))
    {
      timestamp = g_variant_get_uint64(value);
    }
    g_variant_unref(value);
  }
  event_build_string(self, app_name, result, timestamp);
}


static void
parse_event_variant_iter(CcfirmwareSecurityPanel *self, GVariantIter *iter)
{
  GVariant *value;
  const gchar *key;
  gchar *app_name = NULL;
  guint64 flags = 0;
  guint32 hsi_level = 0;

  while (g_variant_iter_next(iter, "{&sv}", &key, &value)) {
    if (!g_strcmp0 (key, "AppstreamId")) {
      app_name = g_variant_get_string (value, NULL);
      if(g_hash_table_lookup(self->event_hash_table, app_name))
      {
        event_on_hit(self, app_name, iter);
        g_variant_unref(value);
        break;
      }else
      {
        event_hash_insert(self, app_name, iter);
        g_variant_unref(value);
        break;
      }
    }
    g_variant_unref(value);
  }
}


static void
parse_variant_iter(CcfirmwareSecurityPanel *self, GVariantIter *iter)
{
  GVariant *value;
  const gchar *key;
  gchar *app_name = NULL;
  guint64 flags = 0;
  guint32 hsi_level = 0;

	while (g_variant_iter_next(iter, "{&sv}", &key, &value)) {
      if (!g_strcmp0 (key, "AppstreamId")) {
        app_name = g_variant_get_string (value, NULL);
      }else if(!g_strcmp0(key, "Flags"))
      {
        flags = g_variant_get_uint64(value);
      }else if(!g_strcmp0 (key, "HsiLevel"))
      {
        hsi_level = g_variant_get_uint32(value);
        switch (hsi_level)
        {
          case 1:
                  g_hash_table_insert(self->hsi1_hash_table,
                                      g_strdup(app_name),
                                      GINT_TO_POINTER(flags));
                  break;
          case 2:
                  g_hash_table_insert(self->hsi2_hash_table,
                                      g_strdup(app_name),
                                      GINT_TO_POINTER(flags));
                  break;
          case 3:
                  g_hash_table_insert(self->hsi3_hash_table,
                                      g_strdup(app_name),
                                      GINT_TO_POINTER(flags));
                  break;
          case 4:
                  g_hash_table_insert(self->hsi4_hash_table,
                                      g_strdup(app_name),
                                      GINT_TO_POINTER(flags));
                  break;
        }
      }
		g_variant_unref(value);
	}
  if(hsi_level == 0 && app_name)
  {
    g_hash_table_insert(self->hsi_general_hash_table,
                        g_strdup(app_name),
                        GINT_TO_POINTER(flags));
  }
}


static void
parse_data_from_variant(CcfirmwareSecurityPanel *self, GVariant *value, const gboolean is_event)
{
	const gchar *type_string;
	g_autoptr(GVariantIter) iter = NULL;

	type_string = g_variant_get_type_string(value);
	if (g_strcmp0(type_string, "(a{sv})") == 0) {
		g_variant_get(value, "(a{sv})", &iter);
    if(is_event)
      parse_event_variant_iter(self, iter);
    else
		  parse_variant_iter(self, iter);
	} else if (g_strcmp0(type_string, "a{sv}") == 0) {
		g_variant_get(value, "a{sv}", &iter);
    if(is_event)
      parse_event_variant_iter(self, iter);
    else
		  parse_variant_iter(self, iter);
	} else {
		g_warning("type %s not known", type_string);
	}
}


static void
parse_array_from_variant (CcfirmwareSecurityPanel *self, GVariant *value, const gboolean is_event)
{
	GPtrArray *array = NULL;
	gsize sz;
	g_autoptr(GVariant) untuple = NULL;

	array = g_ptr_array_new_with_free_func ((GDestroyNotify) g_object_unref);
	untuple = g_variant_get_child_value (value, 0);
	sz = g_variant_n_children (untuple);
	for (guint i = 0; i < sz; i++) {
		g_autoptr(GVariant) data = NULL;
		data = g_variant_get_child_value (untuple, i);
    if(is_event)
  		parse_data_from_variant (self, data, TRUE);
    else
  		parse_data_from_variant (self, data, FALSE);
	}
}


static void
on_bus_event_done  (GObject *source,
                    GAsyncResult *res,
                    gpointer user_data)
{
  g_autoptr(GError) error = NULL;
  g_autoptr(GVariant) val = NULL;
  CcfirmwareSecurityPanel *self;
  self = user_data;

	val = g_dbus_proxy_call_finish(G_DBUS_PROXY(source), res, &error);
  if (val == NULL) {
    g_error("Dbus: Fail on getting Security Attribute Event");
		return;
  }

  parse_array_from_variant(self, val, TRUE);
}


static void
on_bus_done  (GObject *source,
              GAsyncResult *res,
              gpointer user_data)
{
  g_autoptr(GError) error = NULL;
  g_autoptr(GVariant) val = NULL;
  CcfirmwareSecurityPanel *self;

  self = user_data;

	val = g_dbus_proxy_call_finish(G_DBUS_PROXY(source), res, &error);
  if (val == NULL) {
    g_error("Dbus: Fail on getting Security Attribute");
		return;
  }

  parse_array_from_variant(self, val, FALSE);
  set_secure_boot_button_view(self);
}


static void
on_bus_ready (GObject      *source_object,
              GAsyncResult *res,
              gpointer      user_data)
{
  g_autoptr(GError) error = NULL;
  CcfirmwareSecurityPanel *self;
  GDBusProxy *proxy;
  GVariant *params;

  proxy = g_dbus_proxy_new_for_bus_finish (res, &error);
  if (proxy == NULL)
    {
      if (!g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
          g_warning ("Fail to connect fwupd: %s",
                     error->message);
      return;
    }
  self = user_data;
  self->bus_proxy = proxy;
  g_dbus_proxy_call(self->bus_proxy,
                    "GetHostSecurityAttrs",
                    NULL,
                    G_DBUS_CALL_FLAGS_NONE,
                    -1,
                    cc_panel_get_cancellable (CC_PANEL (self)),
                    on_bus_done,
                    self);
  g_dbus_proxy_call(self->bus_proxy,
                    "GetHostSecurityEvents",
                    g_variant_new ("(u)",
                                   100),
                    G_DBUS_CALL_FLAGS_NONE,
                    -1,
                    cc_panel_get_cancellable (CC_PANEL (self)),
                    on_bus_event_done,
                    self);
}


static void 
on_hsi_button_click(GtkWidget *widget, gpointer data)
{
  CcFirmwareSecurityDialog *dialog;
  CcfirmwareSecurityPanel  *self = data;

  dialog = cc_firmware_security_dialog_new("Security Level", self->hsi_number,
                                           self->firmware_security_attrs,
                                           self->hsi1_hash_table,
                                           self->hsi2_hash_table,
                                           self->hsi3_hash_table,
                                           self->hsi4_hash_table);
  gtk_widget_show(dialog);
}


static void
on_secure_boot_button_click(GtkWidget *widget, gpointer data)
{
  CcFirmwareSecurityBootDialog *boot_dialog;
  CcfirmwareSecurityPanel  *self = data;

  boot_dialog = cc_firmware_security_boot_dialog_new(self->is_secure_boot);
  gtk_widget_show(boot_dialog);
}


static gint hsi_parser(const gchar *hsi_string)
{
  gchar tmp[2];
  if(!strncasecmp(hsi_string, "HSI", 3))
  {
    g_sprintf(tmp, "%c", hsi_string[4]);
    return g_ascii_strtoll(tmp, NULL, 10);
  }
  return 0;
}


static void
set_hsi_button_view_contain(CcfirmwareSecurityPanel *self, const gchar *icon_name,
                            gchar *title, const gchar *description)
{
  gtk_image_set_from_icon_name(self->hsi_icon, icon_name, 70);
  gtk_label_set_text(self->hsi_label, title);
  gtk_label_set_text(self->hsi_description, description);
}


static void
set_hsi_button_view(CcfirmwareSecurityPanel *self)
{
  switch(self->hsi_number)
  {
    case 0:
      set_hsi_button_view_contain(self, "security-low-symbolic", "No Protection",
                                  "Exposed to seriours security threats.");
      break;
    case 1:
      set_hsi_button_view_contain(self, "security-low-symbolic", "Minimum Protection",
                                  "Limited protection against simple security threats.");
      break;
    case 2:
      set_hsi_button_view_contain(self, "security-medium-symbolic", "Basic Protection",
                                  "Protected against common security threats.");
      break;
    case 3:
      set_hsi_button_view_contain(self, "security-high-symbolic", "Extended Protection",
                                  "Protected against a wide range of security threats.");
      break;
    case 4:
      set_hsi_button_view_contain(self, "security-high-symbolic", "Comprehensive Protection",
                                  "Protected against a wide range security threats.");
      break;
    default:
      g_warning("An incorrect HSI number range.");
  }

}

static void
on_properties_bus_done  (GObject *source,
                         GAsyncResult *res,
                         gpointer user_data)
{
	g_autoptr(GError) error = NULL;
	g_autoptr(GVariant) val = NULL;
	gchar* hsi_string = NULL;
  const gchar *hsi_number = NULL;
  CcfirmwareSecurityPanel *self;

  self = user_data;

	val = g_dbus_proxy_call_finish(G_DBUS_PROXY(source), res, &error);
  if (val == NULL) {
    g_error("Property fail\n");
		return;
  }
  hsi_number = g_variant_get_data(val);
  self->hsi_number = hsi_parser(hsi_number);
  set_hsi_button_view(self);
}

static void
on_properties_bus_ready (GObject      *source_object,
                         GAsyncResult *res,
                         gpointer      user_data)
{
  g_autoptr(GError) error = NULL;
  CcfirmwareSecurityPanel *self;
  GDBusProxy *proxy;
  GVariant *params;

  proxy = g_dbus_proxy_new_for_bus_finish (res, &error);
  if (proxy == NULL)
    {
      if (!g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
          g_warning ("Fail to connect fwupd: %s",
                     error->message);

      return;
    }
  self = user_data;
  self->properties_bus_proxy = proxy;

  g_dbus_proxy_call(self->properties_bus_proxy,
			              "Get",
			              g_variant_new ("(ss)",
                                   "org.freedesktop.fwupd",
                                   "HostSecurityId"),
			              G_DBUS_CALL_FLAGS_NONE,
			              -1,
			              cc_panel_get_cancellable (CC_PANEL (self)),
			              on_properties_bus_done,
			              self);
}


static void
cc_firmware_security_panel_finalize (GObject *object)
{
  CcfirmwareSecurityPanel *self = CC_FIRMWARE_SECURITY_PANEL (object);
 


  G_OBJECT_CLASS (cc_firmware_security_panel_parent_class)->finalize (object);
}

static const char *
cc_firmware_security_panel_get_help_uri (CcPanel *panel)
{
  return "help:gnome-help/location";
}

static void
cc_firmware_security_panel_class_init (CcfirmwareSecurityPanelClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  CcPanelClass *panel_class = CC_PANEL_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  panel_class->get_help_uri = cc_firmware_security_panel_get_help_uri;

  object_class->finalize = cc_firmware_security_panel_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/control-center/firmware-security/cc-firmware-security-panel.ui");

  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, hsi_button);
  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, hsi_icon);
  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, hsi_label);
  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, hsi_description);

  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, secure_boot_button);
  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, secure_boot_icon);
  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, secure_boot_label);
  gtk_widget_class_bind_template_child (widget_class, CcfirmwareSecurityPanel, secure_boot_description);

  gtk_widget_class_bind_template_callback (widget_class, on_hsi_button_click);
  gtk_widget_class_bind_template_callback (widget_class, on_secure_boot_button_click);
}

static void
cc_firmware_security_panel_init (CcfirmwareSecurityPanel *self)
{
  g_resources_register (cc_firmware_security_get_resource ());
  gtk_widget_init_template (GTK_WIDGET (self));

  self->firmware_security_icon_size_group = gtk_size_group_new (GTK_SIZE_GROUP_BOTH);

  self->firmware_security_attrs = g_hash_table_new_full (g_str_hash,
                                                         g_str_equal,
                                                         g_free,
                                                         g_free);
  self->hsi1_hash_table = g_hash_table_new_full (g_str_hash,
                                                 g_str_equal,
                                                 g_free,
                                                 g_free);
  self->hsi2_hash_table = g_hash_table_new_full (g_str_hash,
                                                 g_str_equal,
                                                 g_free,
                                                 g_free);
  self->hsi3_hash_table = g_hash_table_new_full (g_str_hash,
                                                 g_str_equal,
                                                 g_free,
                                                 g_free);
  self->hsi4_hash_table = g_hash_table_new_full (g_str_hash,
                                                 g_str_equal,
                                                 g_free,
                                                 g_free);
  self->hsi_general_hash_table = g_hash_table_new_full (g_str_hash,
                                                        g_str_equal,
                                                        g_free,
                                                        g_free);
  self->event_hash_table = g_hash_table_new_full (g_str_hash,
                                                  g_str_equal,
                                                  g_free,
                                                  g_free);
  g_dbus_proxy_new_for_bus (G_BUS_TYPE_SYSTEM,  
                            G_DBUS_PROXY_FLAGS_NONE,
                            NULL,
                            "org.freedesktop.fwupd",
                            "/",
                            "org.freedesktop.DBus.Properties",
                            cc_panel_get_cancellable (CC_PANEL (self)),
                            on_properties_bus_ready,
                            self);
  g_dbus_proxy_new_for_bus (G_BUS_TYPE_SYSTEM,
                            G_DBUS_PROXY_FLAGS_NONE,
                            NULL,
                            "org.freedesktop.fwupd",
                            "/",
                            "org.freedesktop.fwupd",
                            cc_panel_get_cancellable (CC_PANEL (self)),
                            on_bus_ready,
                            self);
}
