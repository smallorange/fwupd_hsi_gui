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
#include "cc-firmware-security-boot-dialog.h"



struct _CcFirmwareSecurityBootDialog {
  GtkDialog            parent;

  GtkWidget            *secure_boot_title;
  GtkWidget            *secure_boot_body;

  gboolean             is_secure_boot;

};

G_DEFINE_TYPE (CcFirmwareSecurityBootDialog, cc_firmware_security_boot_dialog, GTK_TYPE_DIALOG)


static void
update_dialog_items(CcFirmwareSecurityBootDialog *self,
                    const gchar *title, const gchar *body)
{
  gtk_label_set_text(self->secure_boot_title, title);
  gtk_label_set_text(self->secure_boot_body, body);
}

static void
update_dialog(CcFirmwareSecurityBootDialog *self)
{
  if(self->is_secure_boot == TRUE)
    update_dialog_items(self, "Secure Boot is Active",
                       "Secure boot prevents malicious software from being loading when device starts.\n\nFor more information, on secure boot, contact the hardware manufactoure or IT support.");
  else
    update_dialog_items(self, "Secure Boot is Inactive",
                        "Secure boot prevents malicious software from being loading when device starts.\n\nFor more information, on secure boot, contact the hardware manufactoure or IT support.");

}

static void
cc_firmware_security_boot_dialog_finalize (GObject *object)
{
  CcFirmwareSecurityBootDialog *dialog = CC_FIRMWARE_SECURITY_BOOT_DIALOG (object);

 /* g_clear_object (&dialog->settings);
  g_clear_object (&dialog->master_settings);
  g_clear_pointer (&dialog->app_id, g_free);
  g_clear_object (&dialog->perm_store);
  */

  G_OBJECT_CLASS (cc_firmware_security_boot_dialog_parent_class)->dispose (object);
}

static void
cc_firmware_security_boot_dialog_class_init (CcFirmwareSecurityBootDialogClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = cc_firmware_security_boot_dialog_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/control-center/firmware-security/cc-firmware-security-boot-dialog.ui");
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityBootDialog, secure_boot_title);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityBootDialog, secure_boot_body);
}

static void
cc_firmware_security_boot_dialog_init (CcFirmwareSecurityBootDialog *dialog)
{
  gtk_widget_init_template (GTK_WIDGET (dialog));
}

CcFirmwareSecurityBootDialog *
cc_firmware_security_boot_dialog_new (gboolean is_secure_boot)
{
  CcFirmwareSecurityBootDialog *dialog;
  dialog = g_object_new (CC_TYPE_FIRMWARE_SECURITY_BOOT_DIALOG,
                         NULL);
  dialog->is_secure_boot = is_secure_boot;
  update_dialog(dialog);

  return dialog;
}
