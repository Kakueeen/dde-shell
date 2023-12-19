// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dockpluginmanagerextension_p.h"
#include "dockpluginmanagerintegration_p.h"

#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor/QWaylandResource>
#include <QtWaylandCompositor/QWaylandCompositor>

PluginSurface::PluginSurface(DockPluginManager* manager, const QString& pluginId, const QString& itemKey, DockPluginManager::SurfaceType surfaceType, QWaylandSurface *surface, const QWaylandResource &resource)
    : m_manager(manager)
    , m_pluginId(pluginId)
    , m_itemKey(itemKey)
    , m_surfaceType(surfaceType)
    , m_surface(surface)
{
    init(resource.resource());
    setExtensionContainer(surface);
    QWaylandCompositorExtension::initialize();
}

QWaylandQuickShellIntegration* PluginSurface::createIntegration(QWaylandQuickShellSurfaceItem *item)
{
    return new DockPluginManagerIntegration(item);
}

QWaylandSurface* PluginSurface::surface() const
{
    return m_surface;
}

QString PluginSurface::pluginId() const
{
    return m_pluginId;
}

QString PluginSurface::itemKey() const
{
    return m_itemKey;
}

QString PluginSurface::contextMenu() const
{
    return m_contextMenu;
}

int32_t PluginSurface::pluginFlags() const
{
    return m_flags;
}

DockPluginManager::SurfaceType PluginSurface::surfaceType() const
{
    return m_surfaceType;
}

void PluginSurface::click(const QString &menuId, uint32_t checked)
{
    send_handle_click(menuId, checked);
}

void PluginSurface::dock_plugin_surface_request_set_applet_visible(Resource *resource, const QString &itemKey, uint32_t visible)
{

}

void PluginSurface::dock_plugin_surface_create_context_menu(Resource *resource, const QString &contextMenu)
{
    if (contextMenu != m_contextMenu) {
        m_contextMenu = contextMenu;   
    }
}

void PluginSurface::dock_plugin_surface_create_dcc_icon(Resource *resource, const QString &dccIcon)
{
    if (dccIcon != m_dccIcon) {
        m_dccIcon = dccIcon;
    }
}

void PluginSurface::dock_plugin_surface_plugin_flags(Resource *resource, int32_t flags)
{
    if (flags != m_flags) {
        m_flags = flags;
    }
}


DockPluginManager::DockPluginManager(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate(compositor)
    , m_dockDisplayMode(0)
    , m_dockPosition(0)
{
}

void DockPluginManager::initialize()
{
    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    init(compositor->display(), 1);
}

uint32_t DockPluginManager::dockPosition() const
{
    return m_dockPosition;
}

void DockPluginManager::setDockPosition(uint32_t position)
{
    if (m_dockPosition == position)
        return;
    m_dockPosition = position;
    send_position_changed(m_dockPosition);
}

uint32_t DockPluginManager::dockDisplayMode() const
{
    return  m_dockDisplayMode;
}

void DockPluginManager::setDockDisplayMode(uint32_t displayMode)
{
    if (m_dockDisplayMode == displayMode)
        return;
    m_dockDisplayMode = displayMode;
    send_display_mode_changed(m_dockDisplayMode);
}

void DockPluginManager::dock_plugin_manager_v1_create_plugin_surface(Resource *resource, const QString &pluginId, const QString &itemKey, uint32_t surfaceType, struct ::wl_resource *surface, uint32_t id)
{
    Q_UNUSED(resource);
    QWaylandSurface *qwaylandSurface = QWaylandSurface::fromResource(surface);

    QWaylandResource shellSurfaceResource(wl_resource_create(resource->client(), &::dock_plugin_surface_interface,
                                                           wl_resource_get_version(resource->handle), id));

    auto plugin = new PluginSurface(this, pluginId, itemKey, static_cast<SurfaceType>(surfaceType), qwaylandSurface, shellSurfaceResource);
    Q_EMIT pluginSurfaceCreated(plugin);
}
