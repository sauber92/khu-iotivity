/*
 * Generated by gdbus-codegen 2.42.1. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef __OBJECT_MANAGER_GLUE_H__
#define __OBJECT_MANAGER_GLUE_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.freedesktop.DBus.ObjectManager */

#define TYPE_OBJECT_MANAGER (object_manager_get_type ())
#define OBJECT_MANAGER(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_OBJECT_MANAGER, ObjectManager))
#define IS_OBJECT_MANAGER(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_OBJECT_MANAGER))
#define OBJECT_MANAGER_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_OBJECT_MANAGER, ObjectManagerIface))

struct _ObjectManager;
typedef struct _ObjectManager ObjectManager;
typedef struct _ObjectManagerIface ObjectManagerIface;

struct _ObjectManagerIface
{
  GTypeInterface parent_iface;


  gboolean (*handle_get_managed_objects) (
    ObjectManager *object,
    GDBusMethodInvocation *invocation);

  void (*interfaces_added) (
    ObjectManager *object,
    const gchar *arg_object,
    GVariant *arg_interfaces);

  void (*interfaces_removed) (
    ObjectManager *object,
    const gchar *arg_object,
    const gchar *const *arg_interfaces);

};

GType object_manager_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *object_manager_interface_info (void);
guint object_manager_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void object_manager_complete_get_managed_objects (
    ObjectManager *object,
    GDBusMethodInvocation *invocation,
    GVariant *objects);



/* D-Bus signal emissions functions: */
void object_manager_emit_interfaces_added (
    ObjectManager *object,
    const gchar *arg_object,
    GVariant *arg_interfaces);

void object_manager_emit_interfaces_removed (
    ObjectManager *object,
    const gchar *arg_object,
    const gchar *const *arg_interfaces);



/* D-Bus method calls: */
void object_manager_call_get_managed_objects (
    ObjectManager *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean object_manager_call_get_managed_objects_finish (
    ObjectManager *proxy,
    GVariant **out_objects,
    GAsyncResult *res,
    GError **error);

gboolean object_manager_call_get_managed_objects_sync (
    ObjectManager *proxy,
    GVariant **out_objects,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define TYPE_OBJECT_MANAGER_PROXY (object_manager_proxy_get_type ())
#define OBJECT_MANAGER_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_OBJECT_MANAGER_PROXY, ObjectManagerProxy))
#define OBJECT_MANAGER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_OBJECT_MANAGER_PROXY, ObjectManagerProxyClass))
#define OBJECT_MANAGER_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_OBJECT_MANAGER_PROXY, ObjectManagerProxyClass))
#define IS_OBJECT_MANAGER_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_OBJECT_MANAGER_PROXY))
#define IS_OBJECT_MANAGER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_OBJECT_MANAGER_PROXY))

typedef struct _ObjectManagerProxy ObjectManagerProxy;
typedef struct _ObjectManagerProxyClass ObjectManagerProxyClass;
typedef struct _ObjectManagerProxyPrivate ObjectManagerProxyPrivate;

struct _ObjectManagerProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  ObjectManagerProxyPrivate *priv;
};

struct _ObjectManagerProxyClass
{
  GDBusProxyClass parent_class;
};

GType object_manager_proxy_get_type (void) G_GNUC_CONST;

void object_manager_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
ObjectManager *object_manager_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
ObjectManager *object_manager_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void object_manager_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
ObjectManager *object_manager_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
ObjectManager *object_manager_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_OBJECT_MANAGER_SKELETON (object_manager_skeleton_get_type ())
#define OBJECT_MANAGER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_OBJECT_MANAGER_SKELETON, ObjectManagerSkeleton))
#define OBJECT_MANAGER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_OBJECT_MANAGER_SKELETON, ObjectManagerSkeletonClass))
#define OBJECT_MANAGER_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_OBJECT_MANAGER_SKELETON, ObjectManagerSkeletonClass))
#define IS_OBJECT_MANAGER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_OBJECT_MANAGER_SKELETON))
#define IS_OBJECT_MANAGER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_OBJECT_MANAGER_SKELETON))

typedef struct _ObjectManagerSkeleton ObjectManagerSkeleton;
typedef struct _ObjectManagerSkeletonClass ObjectManagerSkeletonClass;
typedef struct _ObjectManagerSkeletonPrivate ObjectManagerSkeletonPrivate;

struct _ObjectManagerSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  ObjectManagerSkeletonPrivate *priv;
};

struct _ObjectManagerSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType object_manager_skeleton_get_type (void) G_GNUC_CONST;

ObjectManager *object_manager_skeleton_new (void);


G_END_DECLS

#endif /* __OBJECT_MANAGER_GLUE_H__ */
