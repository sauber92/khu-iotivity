/*
 * Generated by gdbus-codegen 2.42.1. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef __BLUEZ_GLUE_H__
#define __BLUEZ_GLUE_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.GattService1 */

#define TYPE_GATT_SERVICE1 (gatt_service1_get_type ())
#define GATT_SERVICE1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_SERVICE1, GattService1))
#define IS_GATT_SERVICE1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_SERVICE1))
#define GATT_SERVICE1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_GATT_SERVICE1, GattService1Iface))

struct _GattService1;
typedef struct _GattService1 GattService1;
typedef struct _GattService1Iface GattService1Iface;

struct _GattService1Iface
{
  GTypeInterface parent_iface;

  const gchar *const * (*get_characteristics) (GattService1 *object);

  gboolean  (*get_primary) (GattService1 *object);

  const gchar * (*get_uuid) (GattService1 *object);

};

GType gatt_service1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *gatt_service1_interface_info (void);
guint gatt_service1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus property accessors: */
const gchar *gatt_service1_get_uuid (GattService1 *object);
gchar *gatt_service1_dup_uuid (GattService1 *object);
void gatt_service1_set_uuid (GattService1 *object, const gchar *value);

gboolean gatt_service1_get_primary (GattService1 *object);
void gatt_service1_set_primary (GattService1 *object, gboolean value);

const gchar *const *gatt_service1_get_characteristics (GattService1 *object);
gchar **gatt_service1_dup_characteristics (GattService1 *object);
void gatt_service1_set_characteristics (GattService1 *object, const gchar *const *value);


/* ---- */

#define TYPE_GATT_SERVICE1_PROXY (gatt_service1_proxy_get_type ())
#define GATT_SERVICE1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_SERVICE1_PROXY, GattService1Proxy))
#define GATT_SERVICE1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_GATT_SERVICE1_PROXY, GattService1ProxyClass))
#define GATT_SERVICE1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_GATT_SERVICE1_PROXY, GattService1ProxyClass))
#define IS_GATT_SERVICE1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_SERVICE1_PROXY))
#define IS_GATT_SERVICE1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_GATT_SERVICE1_PROXY))

typedef struct _GattService1Proxy GattService1Proxy;
typedef struct _GattService1ProxyClass GattService1ProxyClass;
typedef struct _GattService1ProxyPrivate GattService1ProxyPrivate;

struct _GattService1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  GattService1ProxyPrivate *priv;
};

struct _GattService1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType gatt_service1_proxy_get_type (void) G_GNUC_CONST;

void gatt_service1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GattService1 *gatt_service1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
GattService1 *gatt_service1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void gatt_service1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GattService1 *gatt_service1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GattService1 *gatt_service1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_GATT_SERVICE1_SKELETON (gatt_service1_skeleton_get_type ())
#define GATT_SERVICE1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_SERVICE1_SKELETON, GattService1Skeleton))
#define GATT_SERVICE1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_GATT_SERVICE1_SKELETON, GattService1SkeletonClass))
#define GATT_SERVICE1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_GATT_SERVICE1_SKELETON, GattService1SkeletonClass))
#define IS_GATT_SERVICE1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_SERVICE1_SKELETON))
#define IS_GATT_SERVICE1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_GATT_SERVICE1_SKELETON))

typedef struct _GattService1Skeleton GattService1Skeleton;
typedef struct _GattService1SkeletonClass GattService1SkeletonClass;
typedef struct _GattService1SkeletonPrivate GattService1SkeletonPrivate;

struct _GattService1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  GattService1SkeletonPrivate *priv;
};

struct _GattService1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType gatt_service1_skeleton_get_type (void) G_GNUC_CONST;

GattService1 *gatt_service1_skeleton_new (void);


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.GattCharacteristic1 */

#define TYPE_GATT_CHARACTERISTIC1 (gatt_characteristic1_get_type ())
#define GATT_CHARACTERISTIC1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_CHARACTERISTIC1, GattCharacteristic1))
#define IS_GATT_CHARACTERISTIC1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_CHARACTERISTIC1))
#define GATT_CHARACTERISTIC1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_GATT_CHARACTERISTIC1, GattCharacteristic1Iface))

struct _GattCharacteristic1;
typedef struct _GattCharacteristic1 GattCharacteristic1;
typedef struct _GattCharacteristic1Iface GattCharacteristic1Iface;

struct _GattCharacteristic1Iface
{
  GTypeInterface parent_iface;


  gboolean (*handle_start_notify) (
    GattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_stop_notify) (
    GattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_write_value) (
    GattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_value);

  const gchar *const * (*get_descriptors) (GattCharacteristic1 *object);

  const gchar *const * (*get_flags) (GattCharacteristic1 *object);

  gboolean  (*get_notifying) (GattCharacteristic1 *object);

  const gchar * (*get_service) (GattCharacteristic1 *object);

  const gchar * (*get_uuid) (GattCharacteristic1 *object);

  GVariant * (*get_value) (GattCharacteristic1 *object);

};

GType gatt_characteristic1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *gatt_characteristic1_interface_info (void);
guint gatt_characteristic1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void gatt_characteristic1_complete_write_value (
    GattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void gatt_characteristic1_complete_start_notify (
    GattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void gatt_characteristic1_complete_stop_notify (
    GattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void gatt_characteristic1_call_write_value (
    GattCharacteristic1 *proxy,
    GVariant *arg_value,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gatt_characteristic1_call_write_value_finish (
    GattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gatt_characteristic1_call_write_value_sync (
    GattCharacteristic1 *proxy,
    GVariant *arg_value,
    GCancellable *cancellable,
    GError **error);

void gatt_characteristic1_call_start_notify (
    GattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gatt_characteristic1_call_start_notify_finish (
    GattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gatt_characteristic1_call_start_notify_sync (
    GattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);

void gatt_characteristic1_call_stop_notify (
    GattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gatt_characteristic1_call_stop_notify_finish (
    GattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gatt_characteristic1_call_stop_notify_sync (
    GattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *gatt_characteristic1_get_uuid (GattCharacteristic1 *object);
gchar *gatt_characteristic1_dup_uuid (GattCharacteristic1 *object);
void gatt_characteristic1_set_uuid (GattCharacteristic1 *object, const gchar *value);

const gchar *gatt_characteristic1_get_service (GattCharacteristic1 *object);
gchar *gatt_characteristic1_dup_service (GattCharacteristic1 *object);
void gatt_characteristic1_set_service (GattCharacteristic1 *object, const gchar *value);

GVariant *gatt_characteristic1_get_value (GattCharacteristic1 *object);
GVariant *gatt_characteristic1_dup_value (GattCharacteristic1 *object);
void gatt_characteristic1_set_value (GattCharacteristic1 *object, GVariant *value);

gboolean gatt_characteristic1_get_notifying (GattCharacteristic1 *object);
void gatt_characteristic1_set_notifying (GattCharacteristic1 *object, gboolean value);

const gchar *const *gatt_characteristic1_get_flags (GattCharacteristic1 *object);
gchar **gatt_characteristic1_dup_flags (GattCharacteristic1 *object);
void gatt_characteristic1_set_flags (GattCharacteristic1 *object, const gchar *const *value);

const gchar *const *gatt_characteristic1_get_descriptors (GattCharacteristic1 *object);
gchar **gatt_characteristic1_dup_descriptors (GattCharacteristic1 *object);
void gatt_characteristic1_set_descriptors (GattCharacteristic1 *object, const gchar *const *value);


/* ---- */

#define TYPE_GATT_CHARACTERISTIC1_PROXY (gatt_characteristic1_proxy_get_type ())
#define GATT_CHARACTERISTIC1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_CHARACTERISTIC1_PROXY, GattCharacteristic1Proxy))
#define GATT_CHARACTERISTIC1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_GATT_CHARACTERISTIC1_PROXY, GattCharacteristic1ProxyClass))
#define GATT_CHARACTERISTIC1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_GATT_CHARACTERISTIC1_PROXY, GattCharacteristic1ProxyClass))
#define IS_GATT_CHARACTERISTIC1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_CHARACTERISTIC1_PROXY))
#define IS_GATT_CHARACTERISTIC1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_GATT_CHARACTERISTIC1_PROXY))

typedef struct _GattCharacteristic1Proxy GattCharacteristic1Proxy;
typedef struct _GattCharacteristic1ProxyClass GattCharacteristic1ProxyClass;
typedef struct _GattCharacteristic1ProxyPrivate GattCharacteristic1ProxyPrivate;

struct _GattCharacteristic1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  GattCharacteristic1ProxyPrivate *priv;
};

struct _GattCharacteristic1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType gatt_characteristic1_proxy_get_type (void) G_GNUC_CONST;

void gatt_characteristic1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GattCharacteristic1 *gatt_characteristic1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
GattCharacteristic1 *gatt_characteristic1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void gatt_characteristic1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GattCharacteristic1 *gatt_characteristic1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GattCharacteristic1 *gatt_characteristic1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_GATT_CHARACTERISTIC1_SKELETON (gatt_characteristic1_skeleton_get_type ())
#define GATT_CHARACTERISTIC1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_CHARACTERISTIC1_SKELETON, GattCharacteristic1Skeleton))
#define GATT_CHARACTERISTIC1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_GATT_CHARACTERISTIC1_SKELETON, GattCharacteristic1SkeletonClass))
#define GATT_CHARACTERISTIC1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_GATT_CHARACTERISTIC1_SKELETON, GattCharacteristic1SkeletonClass))
#define IS_GATT_CHARACTERISTIC1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_CHARACTERISTIC1_SKELETON))
#define IS_GATT_CHARACTERISTIC1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_GATT_CHARACTERISTIC1_SKELETON))

typedef struct _GattCharacteristic1Skeleton GattCharacteristic1Skeleton;
typedef struct _GattCharacteristic1SkeletonClass GattCharacteristic1SkeletonClass;
typedef struct _GattCharacteristic1SkeletonPrivate GattCharacteristic1SkeletonPrivate;

struct _GattCharacteristic1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  GattCharacteristic1SkeletonPrivate *priv;
};

struct _GattCharacteristic1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType gatt_characteristic1_skeleton_get_type (void) G_GNUC_CONST;

GattCharacteristic1 *gatt_characteristic1_skeleton_new (void);


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.GattDescriptor1 */

#define TYPE_GATT_DESCRIPTOR1 (gatt_descriptor1_get_type ())
#define GATT_DESCRIPTOR1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_DESCRIPTOR1, GattDescriptor1))
#define IS_GATT_DESCRIPTOR1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_DESCRIPTOR1))
#define GATT_DESCRIPTOR1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_GATT_DESCRIPTOR1, GattDescriptor1Iface))

struct _GattDescriptor1;
typedef struct _GattDescriptor1 GattDescriptor1;
typedef struct _GattDescriptor1Iface GattDescriptor1Iface;

struct _GattDescriptor1Iface
{
  GTypeInterface parent_iface;


  gboolean (*handle_read_value) (
    GattDescriptor1 *object,
    GDBusMethodInvocation *invocation);

  const gchar * (*get_characteristic) (GattDescriptor1 *object);

  const gchar *const * (*get_flags) (GattDescriptor1 *object);

  const gchar * (*get_uuid) (GattDescriptor1 *object);

  GVariant * (*get_value) (GattDescriptor1 *object);

};

GType gatt_descriptor1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *gatt_descriptor1_interface_info (void);
guint gatt_descriptor1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void gatt_descriptor1_complete_read_value (
    GattDescriptor1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *value);



/* D-Bus method calls: */
void gatt_descriptor1_call_read_value (
    GattDescriptor1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gatt_descriptor1_call_read_value_finish (
    GattDescriptor1 *proxy,
    GVariant **out_value,
    GAsyncResult *res,
    GError **error);

gboolean gatt_descriptor1_call_read_value_sync (
    GattDescriptor1 *proxy,
    GVariant **out_value,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *gatt_descriptor1_get_uuid (GattDescriptor1 *object);
gchar *gatt_descriptor1_dup_uuid (GattDescriptor1 *object);
void gatt_descriptor1_set_uuid (GattDescriptor1 *object, const gchar *value);

const gchar *gatt_descriptor1_get_characteristic (GattDescriptor1 *object);
gchar *gatt_descriptor1_dup_characteristic (GattDescriptor1 *object);
void gatt_descriptor1_set_characteristic (GattDescriptor1 *object, const gchar *value);

GVariant *gatt_descriptor1_get_value (GattDescriptor1 *object);
GVariant *gatt_descriptor1_dup_value (GattDescriptor1 *object);
void gatt_descriptor1_set_value (GattDescriptor1 *object, GVariant *value);

const gchar *const *gatt_descriptor1_get_flags (GattDescriptor1 *object);
gchar **gatt_descriptor1_dup_flags (GattDescriptor1 *object);
void gatt_descriptor1_set_flags (GattDescriptor1 *object, const gchar *const *value);


/* ---- */

#define TYPE_GATT_DESCRIPTOR1_PROXY (gatt_descriptor1_proxy_get_type ())
#define GATT_DESCRIPTOR1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_DESCRIPTOR1_PROXY, GattDescriptor1Proxy))
#define GATT_DESCRIPTOR1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_GATT_DESCRIPTOR1_PROXY, GattDescriptor1ProxyClass))
#define GATT_DESCRIPTOR1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_GATT_DESCRIPTOR1_PROXY, GattDescriptor1ProxyClass))
#define IS_GATT_DESCRIPTOR1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_DESCRIPTOR1_PROXY))
#define IS_GATT_DESCRIPTOR1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_GATT_DESCRIPTOR1_PROXY))

typedef struct _GattDescriptor1Proxy GattDescriptor1Proxy;
typedef struct _GattDescriptor1ProxyClass GattDescriptor1ProxyClass;
typedef struct _GattDescriptor1ProxyPrivate GattDescriptor1ProxyPrivate;

struct _GattDescriptor1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  GattDescriptor1ProxyPrivate *priv;
};

struct _GattDescriptor1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType gatt_descriptor1_proxy_get_type (void) G_GNUC_CONST;

void gatt_descriptor1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GattDescriptor1 *gatt_descriptor1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
GattDescriptor1 *gatt_descriptor1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void gatt_descriptor1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GattDescriptor1 *gatt_descriptor1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GattDescriptor1 *gatt_descriptor1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_GATT_DESCRIPTOR1_SKELETON (gatt_descriptor1_skeleton_get_type ())
#define GATT_DESCRIPTOR1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_GATT_DESCRIPTOR1_SKELETON, GattDescriptor1Skeleton))
#define GATT_DESCRIPTOR1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_GATT_DESCRIPTOR1_SKELETON, GattDescriptor1SkeletonClass))
#define GATT_DESCRIPTOR1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_GATT_DESCRIPTOR1_SKELETON, GattDescriptor1SkeletonClass))
#define IS_GATT_DESCRIPTOR1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_GATT_DESCRIPTOR1_SKELETON))
#define IS_GATT_DESCRIPTOR1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_GATT_DESCRIPTOR1_SKELETON))

typedef struct _GattDescriptor1Skeleton GattDescriptor1Skeleton;
typedef struct _GattDescriptor1SkeletonClass GattDescriptor1SkeletonClass;
typedef struct _GattDescriptor1SkeletonPrivate GattDescriptor1SkeletonPrivate;

struct _GattDescriptor1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  GattDescriptor1SkeletonPrivate *priv;
};

struct _GattDescriptor1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType gatt_descriptor1_skeleton_get_type (void) G_GNUC_CONST;

GattDescriptor1 *gatt_descriptor1_skeleton_new (void);


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.LEAdvertisement1 */

#define TYPE_LEADVERTISEMENT1 (leadvertisement1_get_type ())
#define LEADVERTISEMENT1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_LEADVERTISEMENT1, LEAdvertisement1))
#define IS_LEADVERTISEMENT1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_LEADVERTISEMENT1))
#define LEADVERTISEMENT1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_LEADVERTISEMENT1, LEAdvertisement1Iface))

struct _LEAdvertisement1;
typedef struct _LEAdvertisement1 LEAdvertisement1;
typedef struct _LEAdvertisement1Iface LEAdvertisement1Iface;

struct _LEAdvertisement1Iface
{
  GTypeInterface parent_iface;


  gboolean (*handle_release) (
    LEAdvertisement1 *object,
    GDBusMethodInvocation *invocation);

  gboolean  (*get_include_tx_power) (LEAdvertisement1 *object);

  GVariant * (*get_manufacturer_data) (LEAdvertisement1 *object);

  GVariant * (*get_service_data) (LEAdvertisement1 *object);

  const gchar *const * (*get_service_uuids) (LEAdvertisement1 *object);

  const gchar *const * (*get_solicit_uuids) (LEAdvertisement1 *object);

  const gchar * (*get_type_) (LEAdvertisement1 *object);

};

GType leadvertisement1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *leadvertisement1_interface_info (void);
guint leadvertisement1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void leadvertisement1_complete_release (
    LEAdvertisement1 *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void leadvertisement1_call_release (
    LEAdvertisement1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean leadvertisement1_call_release_finish (
    LEAdvertisement1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean leadvertisement1_call_release_sync (
    LEAdvertisement1 *proxy,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *leadvertisement1_get_type_ (LEAdvertisement1 *object);
gchar *leadvertisement1_dup_type_ (LEAdvertisement1 *object);
void leadvertisement1_set_type_ (LEAdvertisement1 *object, const gchar *value);

const gchar *const *leadvertisement1_get_service_uuids (LEAdvertisement1 *object);
gchar **leadvertisement1_dup_service_uuids (LEAdvertisement1 *object);
void leadvertisement1_set_service_uuids (LEAdvertisement1 *object, const gchar *const *value);

GVariant *leadvertisement1_get_manufacturer_data (LEAdvertisement1 *object);
GVariant *leadvertisement1_dup_manufacturer_data (LEAdvertisement1 *object);
void leadvertisement1_set_manufacturer_data (LEAdvertisement1 *object, GVariant *value);

const gchar *const *leadvertisement1_get_solicit_uuids (LEAdvertisement1 *object);
gchar **leadvertisement1_dup_solicit_uuids (LEAdvertisement1 *object);
void leadvertisement1_set_solicit_uuids (LEAdvertisement1 *object, const gchar *const *value);

GVariant *leadvertisement1_get_service_data (LEAdvertisement1 *object);
GVariant *leadvertisement1_dup_service_data (LEAdvertisement1 *object);
void leadvertisement1_set_service_data (LEAdvertisement1 *object, GVariant *value);

gboolean leadvertisement1_get_include_tx_power (LEAdvertisement1 *object);
void leadvertisement1_set_include_tx_power (LEAdvertisement1 *object, gboolean value);


/* ---- */

#define TYPE_LEADVERTISEMENT1_PROXY (leadvertisement1_proxy_get_type ())
#define LEADVERTISEMENT1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_LEADVERTISEMENT1_PROXY, LEAdvertisement1Proxy))
#define LEADVERTISEMENT1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_LEADVERTISEMENT1_PROXY, LEAdvertisement1ProxyClass))
#define LEADVERTISEMENT1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_LEADVERTISEMENT1_PROXY, LEAdvertisement1ProxyClass))
#define IS_LEADVERTISEMENT1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_LEADVERTISEMENT1_PROXY))
#define IS_LEADVERTISEMENT1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_LEADVERTISEMENT1_PROXY))

typedef struct _LEAdvertisement1Proxy LEAdvertisement1Proxy;
typedef struct _LEAdvertisement1ProxyClass LEAdvertisement1ProxyClass;
typedef struct _LEAdvertisement1ProxyPrivate LEAdvertisement1ProxyPrivate;

struct _LEAdvertisement1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  LEAdvertisement1ProxyPrivate *priv;
};

struct _LEAdvertisement1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType leadvertisement1_proxy_get_type (void) G_GNUC_CONST;

void leadvertisement1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
LEAdvertisement1 *leadvertisement1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
LEAdvertisement1 *leadvertisement1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void leadvertisement1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
LEAdvertisement1 *leadvertisement1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
LEAdvertisement1 *leadvertisement1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_LEADVERTISEMENT1_SKELETON (leadvertisement1_skeleton_get_type ())
#define LEADVERTISEMENT1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_LEADVERTISEMENT1_SKELETON, LEAdvertisement1Skeleton))
#define LEADVERTISEMENT1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_LEADVERTISEMENT1_SKELETON, LEAdvertisement1SkeletonClass))
#define LEADVERTISEMENT1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_LEADVERTISEMENT1_SKELETON, LEAdvertisement1SkeletonClass))
#define IS_LEADVERTISEMENT1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_LEADVERTISEMENT1_SKELETON))
#define IS_LEADVERTISEMENT1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_LEADVERTISEMENT1_SKELETON))

typedef struct _LEAdvertisement1Skeleton LEAdvertisement1Skeleton;
typedef struct _LEAdvertisement1SkeletonClass LEAdvertisement1SkeletonClass;
typedef struct _LEAdvertisement1SkeletonPrivate LEAdvertisement1SkeletonPrivate;

struct _LEAdvertisement1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  LEAdvertisement1SkeletonPrivate *priv;
};

struct _LEAdvertisement1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType leadvertisement1_skeleton_get_type (void) G_GNUC_CONST;

LEAdvertisement1 *leadvertisement1_skeleton_new (void);


G_END_DECLS

#endif /* __BLUEZ_GLUE_H__ */
