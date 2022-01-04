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

#include <string.h>
#include <glib/gi18n-lib.h>
#include <glib.h>
#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>

#include "cc-firmware-security-panel.h"
#include "cc-firmware-security-dialog.h"
#include "cc-firmware-security-utils.h"



struct _CcFirmwareSecurityDialog {
  GtkDialog            parent;

  GtkWidget           *firmware_security_dialog_stack;

  GtkWidget           *fw_back_button;

  /* Page0 one*/
  GtkWidget            *firmware_security_dialog_icon;
  GtkWidget            *firmware_security_dialog_title_label;
  GtkWidget            *firmware_security_dialog_body_label;
	GtkWidget            *firmware_security_dialog_min_icon;
  GtkWidget            *firmware_security_dialog_basic_icon;
  GtkWidget            *firmware_security_dialog_extend_icon;

	/* Page 2 attr*/
	GtkWidget						 *firmware_security_dialog_hsi1_listbox;
	GtkWidget						 *firmware_security_dialog_hsi2_listbox;
	GtkWidget						 *firmware_security_dialog_hsi3_listbox;
	GtkWidget						 *firmware_security_dialog_hsi4_listbox;

  /* HSI status information */
  GtkWidget            *firmware_security_dialog_hsi_label;
  gboolean             is_created;

  /* HSI attr hash */
  GHashTable           *hsi_attr_hash;

	GHashTable           *hsi1_hash_table;
  GHashTable           *hsi2_hash_table;
  GHashTable           *hsi3_hash_table;
  GHashTable           *hsi4_hash_table;


  gint                 hsi_number;

};

G_DEFINE_TYPE (CcFirmwareSecurityDialog, cc_firmware_security_dialog, GTK_TYPE_DIALOG)


static void
set_dialog_item_laye1(CcFirmwareSecurityDialog *self, const gchar *icon_name,
                      const gchar *style, const gchar *title, const gchar *body)
{
  gchar strbuf[100];
  g_sprintf(strbuf, "Device conforms to HSI level %d", self->hsi_number);
  gtk_image_set_from_icon_name(self->firmware_security_dialog_icon,
                               icon_name,
                               70);
  gtk_widget_set_name(self->firmware_security_dialog_icon, style);
  gtk_label_set_text(self->firmware_security_dialog_title_label, title);
  gtk_label_set_text(self->firmware_security_dialog_body_label, body);
  gtk_label_set_text(self->firmware_security_dialog_hsi_label, strbuf);
}

static void
update_dialog(CcFirmwareSecurityDialog *self)
{
  gtk_image_set_from_icon_name(self->firmware_security_dialog_min_icon,
                               "dialog-error-symbolic",
                               GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_widget_set_name(self->firmware_security_dialog_min_icon, "color_dim");
  gtk_image_set_from_icon_name(self->firmware_security_dialog_basic_icon,
                               "dialog-error-symbolic",
                               GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_widget_set_name(self->firmware_security_dialog_basic_icon, "color_dim");
  gtk_image_set_from_icon_name(self->firmware_security_dialog_extend_icon,
                               "dialog-error-symbolic",
                               GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_widget_set_name(self->firmware_security_dialog_extend_icon, "color_dim");
  
  switch(self->hsi_number)
  {
    case 0:
            set_dialog_item_laye1(self,
                                  "dialog-warning-symbolic",
                                  "icon_error",
                                  _("No Protection"),
                                  _("No Protection description"));

            break;
    case 1:
            gtk_image_set_from_icon_name(self->firmware_security_dialog_min_icon,
                               "emblem-default-symbolic",
                               GTK_ICON_SIZE_SMALL_TOOLBAR);
            gtk_widget_set_name(self->firmware_security_dialog_min_icon, "color_green");
            set_dialog_item_laye1(self,
                                  "security-low-symbolic",
                                  "icon_neutral",
                                  _("Basic Protection"),
                                  _("Basic Protection description"));
            break;
    case 2:
            gtk_image_set_from_icon_name(self->firmware_security_dialog_basic_icon,
                                         "emblem-default-symbolic",
                                         GTK_ICON_SIZE_SMALL_TOOLBAR);
            gtk_widget_set_name(self->firmware_security_dialog_basic_icon, "color_green");
            set_dialog_item_laye1(self,
                                  "security-medium-symbolic",
                                  "icon_warning",
                                  _("Medium Protection"),
                                  _("Minimum Protection description"));
            break;
    case 3:
    case 4:
            gtk_image_set_from_icon_name(self->firmware_security_dialog_extend_icon,
                                         "emblem-default-symbolic",
                                         GTK_ICON_SIZE_SMALL_TOOLBAR);
            gtk_widget_set_name(self->firmware_security_dialog_extend_icon, "color_green");
            set_dialog_item_laye1(self,
                                  "security-high-symbolic",
                                  "icon_good",
                                  _("Extended Protection"),
                                  _("Extended Protection description"));
            break;
    default:
            set_dialog_item_laye1(self,
                                  "dialog-warning-symbolic",
                                  "icon_error",
                                  _("Incorrect HSI"),
                                  _("Incorrect HSI description"));
  }
}

static GtkWidget *
create_row(const gchar *icon_name, const gchar* style, const gchar *item_name)
{
  GtkWidget *box;
  GtkWidget *row;
  GtkWidget *icon;
  GtkWidget *label;

  row = gtk_list_box_row_new ();
  box = gtk_hbox_new(FALSE, 15);
  gtk_list_box_row_set_selectable(row, FALSE);
  g_object_set (box, "margin-start", 10, "margin-end", 10, "margin-top", 5, "margin-bottom", 5, NULL);
  gtk_container_add (GTK_CONTAINER (row), box);
  icon = gtk_image_new_from_icon_name(icon_name, gtk_icon_size_from_name("GTK_ICON_SIZE_LARGE_TOOLBAR"));
  gtk_widget_set_name(icon, style);
  gtk_image_set_pixel_size(icon, 16);
  label = gtk_label_new(fu_security_attr_get_name(item_name));
  gtk_box_pack_start(box, icon, FALSE, TRUE, 10);
  gtk_box_pack_start(box, label, FALSE, TRUE, 10);

  return row;
}


static void
update_hsi_listbox(CcFirmwareSecurityDialog *self, const gint hsi_level)
{
	GHashTable *hsi_hash;
	GList *hash_keys;
  GList *item;  
	GtkWidget *row;
	GtkWidget	*hsi_listbox;
  guint64 flags = 0;

	switch(hsi_level)
	{
		case 1:
            hsi_hash = self->hsi1_hash_table;
            hsi_listbox = self->firmware_security_dialog_hsi1_listbox;
						break;
		case 2:
		        hsi_hash = self->hsi2_hash_table;
						hsi_listbox = self->firmware_security_dialog_hsi2_listbox;
						break;
		case 3:
		        hsi_hash = self->hsi3_hash_table;
						hsi_listbox = self->firmware_security_dialog_hsi3_listbox;
						break;
		case 4:
		        hsi_hash = self->hsi4_hash_table;
						hsi_listbox = self->firmware_security_dialog_hsi4_listbox;
						break;
	}

  hash_keys = g_hash_table_get_keys(hsi_hash);

  for(item = g_list_first(hash_keys); item != NULL; item = g_list_next(item))
	{
    flags =  GPOINTER_TO_INT(g_hash_table_lookup(hsi_hash, item->data));
    if(firmware_security_attr_has_flag(flags, FWUPD_SECURITY_ATTR_FLAG_SUCCESS))
      row = create_row("emblem-default-symbolic", "color_green", item->data);
    else
      row = create_row("dialog-error-symbolic", "color_dim", item->data);
    gtk_list_box_insert(hsi_listbox, row, -1);      
  }
  self->is_created = TRUE;

}


static void
on_hsi_click(GtkWidget *widget, gpointer data)
{
  GList *hash_keys;
  GList *item;
  CcFirmwareSecurityDialog *self;
  GtkWidget *row;
  gint position = 0;
  guint64 status;

  self = data;
  gtk_stack_set_visible_child_name(self->firmware_security_dialog_stack, "page2");
	if(!self->is_created)
	{
    update_hsi_listbox(self, 1);
    update_hsi_listbox(self, 2);
    update_hsi_listbox(self, 3);
    update_hsi_listbox(self, 4);
		self->is_created = TRUE;
	}

  if(!g_strcmp0(gtk_widget_get_name(widget), "firmware_security_min_button"))
    gtk_widget_show_all (self->firmware_security_dialog_hsi1_listbox);
  else if(!g_strcmp0(gtk_widget_get_name(widget), "firmware_security_basic_button"))
    gtk_widget_show_all (self->firmware_security_dialog_hsi2_listbox);
  else if(!g_strcmp0(gtk_widget_get_name(widget), "firmware_security_extend_button"))
  {
    gtk_widget_show_all (self->firmware_security_dialog_hsi3_listbox);
    gtk_widget_show_all (self->firmware_security_dialog_hsi4_listbox);
  }
  gtk_widget_set_visible(self->fw_back_button, TRUE);
}

static void
on_fw_back_button_click(GtkWidget *widget, gpointer data)
{
  CcFirmwareSecurityDialog *self;

  self = data;
  gtk_stack_set_visible_child_name(self->firmware_security_dialog_stack, "page0");
  gtk_widget_set_visible(self->fw_back_button, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi1_listbox, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi2_listbox, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi3_listbox, FALSE);
  gtk_widget_set_visible (self->firmware_security_dialog_hsi4_listbox, FALSE);
}


static void
cc_firmware_security_dialog_finalize (GObject *object)
{
  CcFirmwareSecurityDialog *dialog = CC_FIRMWARE_SECURITY_DIALOG (object);
 
  G_OBJECT_CLASS (cc_firmware_security_dialog_parent_class)->dispose (object);
}

static void
cc_firmware_security_dialog_class_init (CcFirmwareSecurityDialogClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = cc_firmware_security_dialog_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/control-center/firmware-security/cc-firmware-security-dialog.ui");

  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_icon);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_title_label);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_body_label);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi_label);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_stack);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_min_icon);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_basic_icon);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_extend_icon);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi1_listbox);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi2_listbox);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi3_listbox);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, firmware_security_dialog_hsi4_listbox);
	gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityDialog, fw_back_button);

  gtk_widget_class_bind_template_callback (widget_class, on_hsi_click);
  gtk_widget_class_bind_template_callback (widget_class, on_fw_back_button_click);
}

void
cc_firmware_security_dialog_init (CcFirmwareSecurityDialog *dialog)
{
  gtk_widget_init_template (GTK_WIDGET (dialog));
  load_custom_css ("/org/gnome/control-center/firmware-security/security-level.css");
  update_dialog(dialog);
}

CcFirmwareSecurityDialog *
cc_firmware_security_dialog_new (const gchar      *title,
                                 const gint       hsi_number,
                                 const GHashTable *attr_hash,
																 const GHashTable *hsi1_hash_table,
  															 const GHashTable *hsi2_hash_table,
  															 const GHashTable *hsi3_hash_table,
  															 const GHashTable *hsi4_hash_table)
{
  CcFirmwareSecurityDialog *dialog;
  dialog = g_object_new (CC_TYPE_FIRMWARE_SECURITY_DIALOG,
                         NULL);
  gtk_window_set_title (GTK_WINDOW (dialog), title);
  dialog->hsi_number = hsi_number;
  dialog->hsi_attr_hash = attr_hash;
  dialog->is_created = FALSE;
	dialog->hsi1_hash_table = hsi1_hash_table;
  dialog->hsi2_hash_table = hsi2_hash_table;
  dialog->hsi3_hash_table = hsi3_hash_table;
  dialog->hsi4_hash_table = hsi4_hash_table;

  GList *item;
	GList *hash_keys = g_hash_table_get_keys(dialog->hsi2_hash_table );

  return dialog;
}