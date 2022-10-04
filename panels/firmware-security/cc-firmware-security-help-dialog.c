/* cc-firmware-security-dialog.c
 *
 * Copyright (C) 2022 Red Hat, Inc
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
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "config.h"

#include <glib/gi18n-lib.h>

#include "cc-firmware-security-help-dialog.h"
#include "cc-firmware-security-utils.h"

struct _CcFirmwareSecurityHelpDialog
{
  GtkDialog parent;
};

G_DEFINE_TYPE (CcFirmwareSecurityHelpDialog, cc_firmware_security_help_dialog, GTK_TYPE_DIALOG)


static void
cc_firmware_security_help_dialog_class_init (CcFirmwareSecurityHelpDialogClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/control-center/firmware-security/cc-firmware-security-help-dialog.ui");
}

static void
cc_firmware_security_help_dialog_init (CcFirmwareSecurityHelpDialog *dialog)
{
  gtk_widget_init_template (GTK_WIDGET (dialog));
  load_custom_css ("/org/gnome/control-center/firmware-security/security-level.css");
}

GtkWidget *
cc_firmware_security_help_dialog_new (void *args)
{
  CcFirmwareSecurityHelpDialog *dialog;

  dialog = g_object_new (CC_TYPE_FIRMWARE_SECURITY_HELP_DIALOG,
                         "use-header-bar", TRUE,
                         NULL);
  return GTK_WIDGET (dialog);
}
