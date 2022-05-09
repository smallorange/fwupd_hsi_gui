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

#include "cc-firmware-security-boot-dialog.h"

struct _CcFirmwareSecurityBootDialog
{
  GtkDialog         parent;

  GtkWidget        *secure_boot_icon;
  GtkWidget        *secure_boot_title;
  GtkWidget        *secure_boot_body;

  SecureBootState   secure_boot_state;
};

G_DEFINE_TYPE (CcFirmwareSecurityBootDialog, cc_firmware_security_boot_dialog, GTK_TYPE_DIALOG)

static void
update_dialog (CcFirmwareSecurityBootDialog *self)
{
  g_autofree gchar *body = NULL;

  if (self->secure_boot_state == SECURE_BOOT_STATE_ACTIVE)
    {
      /* TRANSLATORS: secure boot refers to the system firmware security mode */
      gtk_label_set_text (GTK_LABEL(self->secure_boot_title), _("Secure Boot is Active"));
      gtk_widget_set_name (self->secure_boot_icon, "icon_good");
    }
  else if (self->secure_boot_state == SECURE_BOOT_STATE_PROBLEMS)
    {
      /* TRANSLATORS: secure boot refers to the system firmware security mode */
      gtk_label_set_text (GTK_LABEL (self->secure_boot_title), _("Secure Boot has Problems"));
      gtk_widget_set_name (self->secure_boot_icon, "icon_error");
    }
  else
    {
      /* TRANSLATORS: secure boot refers to the system firmware security mode */
      gtk_label_set_text (GTK_LABEL (self->secure_boot_title), _("Secure Boot is Inactive"));
      gtk_widget_set_name (self->secure_boot_icon, "icon_error");
    }

  gtk_label_set_text (GTK_LABEL (self->secure_boot_body),
                      /* TRANSLATORS: this is provided by the system firmware */
                      _("Secure boot provides protection against malicious software that embeds "
                        "itself in a computer's firmware when it is started. Secure boot prevents "
                        "this by verifying that firmware has not been tampered with before it is "
                        "loaded."
                        "\n"
                        "\n"
                        "Secure boot can often be enabled or disabled from your computer's UEFI "
                        "firmware settings (BIOS). In some cases, the firmware settings can also "
                        "be used to resolve configuration issues."
                        "\n"
                        "\n"
                        "If secure boot is not functional on your device, it is recommended that "
                        "you contact an IT support provider or your hardware manufacturer."));

}

static void
cc_firmware_security_boot_dialog_class_init (CcFirmwareSecurityBootDialogClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/control-center/firmware-security/cc-firmware-security-boot-dialog.ui");
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
cc_firmware_security_boot_dialog_new (SecureBootState secure_boot_state)
{
  CcFirmwareSecurityBootDialog *dialog;
  dialog = g_object_new (CC_TYPE_FIRMWARE_SECURITY_BOOT_DIALOG, "use-header-bar", TRUE, NULL);
  dialog->secure_boot_state = secure_boot_state;
  update_dialog (dialog);

  return dialog;
}
