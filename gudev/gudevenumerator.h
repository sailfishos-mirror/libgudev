/* -*- Mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * SPDX-FileCopyrightText: 2008-2010 David Zeuthen <davidz@redhat.com>
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#if !defined (_GUDEV_COMPILATION) && !defined(_GUDEV_INSIDE_GUDEV_H)
#error "Only <gudev/gudev.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __G_UDEV_ENUMERATOR_H__
#define __G_UDEV_ENUMERATOR_H__

#include <gudev/gudevtypes.h>

G_BEGIN_DECLS

#define G_UDEV_TYPE_ENUMERATOR         (g_udev_enumerator_get_type ())
#define G_UDEV_ENUMERATOR(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_UDEV_TYPE_ENUMERATOR, GUdevEnumerator))
#define G_UDEV_ENUMERATOR_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_UDEV_TYPE_ENUMERATOR, GUdevEnumeratorClass))
#define G_UDEV_IS_ENUMERATOR(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_UDEV_TYPE_ENUMERATOR))
#define G_UDEV_IS_ENUMERATOR_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_UDEV_TYPE_ENUMERATOR))
#define G_UDEV_ENUMERATOR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_UDEV_TYPE_ENUMERATOR, GUdevEnumeratorClass))

#ifdef G_DEFINE_AUTOPTR_CLEANUP_FUNC
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GUdevEnumerator, g_object_unref)
#endif

typedef struct _GUdevEnumeratorClass   GUdevEnumeratorClass;
typedef struct _GUdevEnumeratorPrivate GUdevEnumeratorPrivate;

// The #GUdevEnumerator struct is opaque and should not be accessed directly.
struct _GUdevEnumerator
{
  GObject              parent;

  /*< private >*/
  GUdevEnumeratorPrivate *priv;
};

/**
 * GUdevEnumeratorClass:
 * @parent_class: Parent class.
 *
 * Class structure for #GUdevEnumerator.
 *
 * Since: 165
 */
struct _GUdevEnumeratorClass
{
  GObjectClass   parent_class;

  /*< private >*/
  /* Padding for future expansion */
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
  void (*reserved4) (void);
  void (*reserved5) (void);
  void (*reserved6) (void);
  void (*reserved7) (void);
  void (*reserved8) (void);
};

GType            g_udev_enumerator_get_type                     (void) G_GNUC_CONST;
GUdevEnumerator *g_udev_enumerator_new                          (GUdevClient      *client);
GUdevEnumerator *g_udev_enumerator_add_match_subsystem          (GUdevEnumerator  *enumerator,
                                                                 const gchar      *subsystem);
GUdevEnumerator *g_udev_enumerator_add_nomatch_subsystem        (GUdevEnumerator  *enumerator,
                                                                 const gchar      *subsystem);
GUdevEnumerator *g_udev_enumerator_add_match_sysfs_attr         (GUdevEnumerator  *enumerator,
                                                                 const gchar      *name,
                                                                 const gchar      *value);
GUdevEnumerator *g_udev_enumerator_add_nomatch_sysfs_attr       (GUdevEnumerator  *enumerator,
                                                                 const gchar      *name,
                                                                 const gchar      *value);
GUdevEnumerator *g_udev_enumerator_add_match_property           (GUdevEnumerator  *enumerator,
                                                                 const gchar      *name,
                                                                 const gchar      *value);
GUdevEnumerator *g_udev_enumerator_add_match_name               (GUdevEnumerator  *enumerator,
                                                                 const gchar      *name);
GUdevEnumerator *g_udev_enumerator_add_match_tag                (GUdevEnumerator  *enumerator,
                                                                 const gchar      *tag);
GUdevEnumerator *g_udev_enumerator_add_match_is_initialized     (GUdevEnumerator  *enumerator);
GUdevEnumerator *g_udev_enumerator_add_sysfs_path               (GUdevEnumerator  *enumerator,
                                                                 const gchar      *sysfs_path);
GList           *g_udev_enumerator_execute                      (GUdevEnumerator  *enumerator);

G_END_DECLS

#endif /* __G_UDEV_ENUMERATOR_H__ */
