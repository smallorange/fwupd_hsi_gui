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

#include "config.h"

#include <glib/gi18n-lib.h>

#include "cc-firmware-security-panel.h"
#include "cc-firmware-security-dialog.h"
#include "cc-firmware-security-utils.h"

struct _CcFirmwareSecurityDialog
{
  GtkDialog            parent;

  GtkWidget           *firmware_security_dialog_stack;

  GtkWidget           *fw_back_button;

  /* Page0 */
  GtkWidget           *firmware_security_dialog_icon;
  GtkWidget           *firmware_security_dialog_title_label;
  GtkWidget           *firmware_security_dialog_body_label;
  GtkWidget           *firmware_security_dialog_min_row;
  GtkWidget           *firmware_security_dialog_basic_row;
  GtkWidget           *firmware_security_dialog_extend_row;

  /* Page 2 attr */
  GtkWidget           *firmware_security_dialog_hsi1_pg;
  GtkWidget           *firmware_security_dialog_hsi2_pg;
  GtkWidget           *firmware_security_dialog_hsi3_pg;

  /* HSI status information */
  GtkWidget           *firmware_security_dialog_hsi_label;
  gboolean             is_created;

  /* HSI attr hash */
  GHashTable          *hsi1_dict;
  GHashTable          *hsi2_dict;
  GHashTable          *hsi3_dict;
  GHashTable          *hsi4_dict;

  guint                hsi_number;
};

G_DEFINE_TYPE (CcFirmwareSecurityDialog, cc_firmware_security_dialog, GTK_TYPE_DIALOG)

static void
set_dialog_item_layer1 (CcFirmwareSecurityDialog *self,
                        const gchar              *icon_name,
                        const gchar              *style,
                        const gchar              *title,
                        const gchar              *body)
{
  g_autofree gchar *str = NULL;

  /* TRANSLATORS: HSI stands for Host Security ID and device refers to the computer as a whole */
  str = g_strdup_printf (_("Device conforms to HSI level %d"), self->hsi_number);
  gtk_image_set_from_icon_name (GTK_IMAGE (self->firmware_security_dialog_icon), icon_name);
  gtk_widget_add_css_class (self->firmware_security_dialog_icon, style);
  gtk_label_set_text (GTK_LABEL (self->firmware_security_dialog_title_label), title);
  gtk_label_set_text (GTK_LABEL (self->firmware_security_dialog_body_label), body);
  gtk_label_set_text (GTK_LABEL (self->firmware_security_dialog_hsi_label), str);
}

static void
update_dialog(CcFirmwareSecurityDialog *self)
{
  adw_action_row_set_icon_name (ADW_ACTION_ROW (self->firmware_security_dialog_min_row), "dialog-error-symbolic");
  adw_action_row_set_icon_name (ADW_ACTION_ROW (self->firmware_security_dialog_basic_row), "dialog-error-symbolic");
  adw_action_row_set_icon_name (ADW_ACTION_ROW (self->firmware_security_dialog_extend_row), "dialog-error-symbolic");
  gtk_widget_add_css_class (self->firmware_security_dialog_min_row, "gray_icon_color");
  gtk_widget_add_css_class (self->firmware_security_dialog_basic_row, "gray_icon_color");
  gtk_widget_add_css_class (self->firmware_security_dialog_extend_row, "gray_icon_color");

  switch (self->hsi_number)
    {
      case 0:
        set_dialog_item_layer1 (self,
                                "dialog-warning-symbolic",
                                "error",
                                _("Security Level: No Protection"),
                                _("This device has no protection against hardware security issues. This could "
                                  "be because of a hardware or firmware configuration issue. It is "
                                  "recommended to contact your IT support provider."));
        break;
      case 1:
        adw_action_row_set_icon_name (ADW_ACTION_ROW (self->firmware_security_dialog_min_row),
                                      "emblem-default-symbolic");
        gtk_widget_add_css_class (self->firmware_security_dialog_min_row, "success_icon_color");
        set_dialog_item_layer1 (self,
                                "security-low-symbolic",
                                "neutral",
                                _("Security Leve: Minimum Protection"),
                                _("This device has minimal protection against hardware security issues. This "
                                  "is the lowest device security level and only provides protection against "
                                  "simple security threats."));
        break;
      case 2:
        adw_action_row_set_icon_name (ADW_ACTION_ROW(self->firmware_security_dialog_basic_row),
                                      "emblem-default-symbolic");
        gtk_widget_add_css_class (self->firmware_security_dialog_basic_row, "success_icon_color");
        set_dialog_item_layer1 (self,
                                "security-medium-symbolic",
                                "warning",
                                _("Security Level: Basic Protection"),
                                _("This device has basic protection against hardware security issues. This "
                                  "provides protection against some common security threats."));
        break;
      case 3:
      case 4:
        adw_action_row_set_icon_name (ADW_ACTION_ROW(self->firmware_security_dialog_extend_row),
                                      "emblem-default-symbolic");
        gtk_widget_add_css_class (self->firmware_security_dialog_extend_row, "success_icon_color");
        set_dialog_item_layer1 (self,
                                "security-high-symbolic",
                                "good",
                                _("Security Level: Extended Protection"),
                                _("This device has extended protection against hardware security issues. This "
                                  "is the highest device security level and provides protection against "
                                  "advanced security threats."));
        break;
      default:
        set_dialog_item_layer1 (self,
                                "dialog-warning-symbolic",
                                "error",
                                _("Error: unable to determine HSI level."),
                                _("Error: unable to determine Incorrect HSI level."));
    }
}

static GtkWidget *
hsi_create_pg_row (const gchar *icon_name,
                   const gchar *style,
                   const gchar *item_name)
{
  GtkWidget *row;

  row = adw_action_row_new ();
  adw_action_row_set_icon_name (ADW_ACTION_ROW (row), icon_name);
  adw_preferences_row_set_title (ADW_PREFERENCES_ROW (row), fu_security_attr_get_name (item_name));

  return row;
}

static void
update_hsi_listbox (CcFirmwareSecurityDialog *self, const gint hsi_level)
{
  GHashTable *hsi_dict = NULL;
  GtkWidget *pg_row;
  GtkWidget *hsi_pg;
  guint64 flags = 0;
  g_autoptr (GList) hash_keys = NULL;

  switch(hsi_level)
    {
      case 1:
        hsi_dict = self->hsi1_dict;
        hsi_pg = self->firmware_security_dialog_hsi1_pg;
        break;
      case 2:
        hsi_dict = self->hsi2_dict;
        hsi_pg = self->firmware_security_dialog_hsi2_pg;
        break;
      case 3:
        hsi_dict = self->hsi3_dict;
        hsi_pg = self->firmware_security_dialog_hsi3_pg;
        break;
      case 4:
        hsi_dict = self->hsi4_dict;
        hsi_pg = self->firmware_security_dialog_hsi3_pg;
        break;
    }

  hash_keys = g_hash_table_get_keys (hsi_dict);
  for(GList *item = g_list_first (hash_keys); item != NULL; item = g_list_next (item))
    {
      flags = GPOINTER_TO_INT(g_hash_table_lookup (hsi_dict, item->data));
      if (firmware_security_attr_has_flag (flags, FWUPD_SECURITY_ATTR_FLAG_SUCCESS))
        {
          pg_row = hsi_create_pg_row ("emblem-default-symbolic", "color_green", item->data);
          gtk_widget_add_css_class (pg_row, "success_icon_color");
        }
      else
        {
          pg_row = hsi_create_pg_row ("dialog-error-symbolic", "color_dim", item->data);
          gtk_widget_add_css_class (pg_row, "error_icon_color");
        }
      adw_preferences_group_add (ADW_PREFERENCES_GROUP (hsi_pg), GTK_WIDGET (pg_row));
    }
  self->is_created = TRUE;
}

static void
on_hsi_clicked_cb (GtkWidget *widget, gpointer data)
{
  CcFirmwareSecurityDialog *self = CC_FIRMWARE_SECURITY_DIALOG (data);

  adw_view_stack_set_visible_child_name (ADW_VIEW_STACK (self->firmware_security_dialog_stack), "page2");
  if (!self->is_created)
    {
      update_hsi_listbox (self, 1);
      update_hsi_listbox (self, 2);
      update_hsi_listbox (self, 3);
      update_hsi_listbox (self, 4);
      self->is_created = TRUE;
    }

  if (g_strcmp0 (gtk_widget_get_name (widget), "firmware_security_min_button") == 0)
    gtk_widget_set_visible (self->firmware_security_dialog_hsi1_pg, TRUE);
  else if (g_strcmp0(gtk_widget_get_name (widget), "firmware_security_basic_button") == 0)
    gtk_widget_set_visible (self->firmware_security_dialog_hsi2_pg, TRUE);
  else if (g_strcmp0 (gtk_widget_get_name (widget), "firmware_security_extend_button") == 0)
    gtk_widget_set_visible (self->firmware_security_dialog_hsi3_pg, TRUE);

  gtk_widget_set_visible (self->fw_back_button, TRUE);
}

static void
on_fw_back_button_clicked_cb (GtkWidget *widget, gpointer data)
{
  CcFirmwareSecurityDialog *self = CC_FIRMWARE_SECURITY_DIALOG (data);

  adw_view_stack_set_visible_child_name (ADW_VIEW_STACK (self->firmware_security_dialog_stack), "page0");
  gtk_widget_set_visible (self->fw_back_button, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi1_pg, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi2_pg, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi3_pg, FALSE);
}

static void
cc_firmware_security_dialog_class_init (CcFirmwareSecurityDialogClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/control-center/firmware-security/cc-firmware-security-dialog.ui");

  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_icon);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_title_label);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_body_label);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi_label);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_stack);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_min_row);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_basic_row);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_extend_row);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi1_pg);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi2_pg);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi3_pg);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, fw_back_button);

  gtk_widget_class_bind_template_callback (widget_class, on_hsi_clicked_cb);
  gtk_widget_class_bind_template_callback (widget_class, on_fw_back_button_clicked_cb);
}

void
cc_firmware_security_dialog_init (CcFirmwareSecurityDialog *dialog)
{
  gtk_widget_init_template (GTK_WIDGET (dialog));
  load_custom_css ("/org/gnome/control-center/firmware-security/security-level.css");
}

GtkWidget *
cc_firmware_security_dialog_new (guint       hsi_number,
                                 GHashTable *hsi1_dict,
                                 GHashTable *hsi2_dict,
                                 GHashTable *hsi3_dict,
                                 GHashTable *hsi4_dict)
{
  CcFirmwareSecurityDialog *dialog;
  dialog = g_object_new (CC_TYPE_FIRMWARE_SECURITY_DIALOG,
                         "use-header-bar",
                         TRUE,
                         NULL);
  dialog->hsi_number = hsi_number;
  dialog->is_created = FALSE;
  dialog->hsi1_dict = hsi1_dict;
  dialog->hsi2_dict = hsi2_dict;
  dialog->hsi3_dict = hsi3_dict;
  dialog->hsi4_dict = hsi4_dict;
  update_dialog (dialog);

  return GTK_WIDGET (dialog);
}
