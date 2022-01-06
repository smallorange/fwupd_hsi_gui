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
#include "cc-firmware-security-utils.h"

struct _CcFirmwareSecurityBootDialog {
  GtkDialog            parent;

  GtkWidget            *secure_boot_icon;
  GtkWidget            *secure_boot_title;
  GtkWidget            *secure_boot_body;

  gboolean             is_secure_boot;

};

G_DEFINE_TYPE (CcFirmwareSecurityBootDialog, cc_firmware_security_boot_dialog, GTK_TYPE_DIALOG)


static void
update_dialog_items(CcFirmwareSecurityBootDialog *self,
                    const gchar *title, const gchar *body)
{
  gtk_label_set_text(GTK_LABEL(self->secure_boot_title), title);
  gtk_label_set_text(GTK_LABEL(self->secure_boot_body), body);
}

static void
update_dialog(CcFirmwareSecurityBootDialog *self)
{
  if(self->is_secure_boot == TRUE)
  {
    update_dialog_items(self, _("Secure Boot is Active"),
                        _("Secure boot active description"));
    gtk_widget_set_name(self->secure_boot_icon, "icon_good");
  } else {
    update_dialog_items(self, _("Secure Boot is Inactive"),
                        _("Secure boot inactive description"));
    gtk_widget_set_name(self->secure_boot_icon, "icon_error");
  }
}

static void
cc_firmware_security_boot_dialog_finalize (GObject *object)
{
  G_OBJECT_CLASS (cc_firmware_security_boot_dialog_parent_class)->dispose (object);
}

static void
cc_firmware_security_boot_dialog_class_init (CcFirmwareSecurityBootDialogClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = cc_firmware_security_boot_dialog_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, 
                "/org/gnome/control-center/firmware-security/cc-firmware-security-boot-dialog.ui");
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityBootDialog, secure_boot_title);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityBootDialog, secure_boot_body);
  gtk_widget_class_bind_template_child (widget_class, CcFirmwareSecurityBootDialog, secure_boot_icon);
}

static void
cc_firmware_security_boot_dialog_init (CcFirmwareSecurityBootDialog *dialog)
{
  gtk_widget_init_template (GTK_WIDGET (dialog));
  load_custom_css ("/org/gnome/control-center/firmware-security/security-level.css");
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
