package me.example.auth;

import org.bukkit.Location;

import java.util.Set;

import org.bukkit.GameMode;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.player.PlayerJoinEvent;
import org.bukkit.event.player.PlayerMoveEvent;
import org.bukkit.event.player.PlayerQuitEvent;
import org.bukkit.event.player.PlayerCommandPreprocessEvent;
import org.bukkit.event.EventPriority;

public class AuthListener implements Listener {

    private final AuthPlugin plugin;

    public AuthListener(AuthPlugin plugin) {
        this.plugin = plugin;
    }

    @EventHandler
    public void onJoin(PlayerJoinEvent event) {

        event.getPlayer().setGameMode(GameMode.SPECTATOR);

        Location lobby = new Location(
                event.getPlayer().getWorld(),
                0,
                100,
                0
        );

        event.getPlayer().teleport(lobby);

        event.getPlayer().sendMessage(
                "§eВведите /login для входа или /register для регистрации"
        );
    }

    @EventHandler
    public void onMove(PlayerMoveEvent event) {

        if (plugin.isAuthenticated(
                event.getPlayer().getUniqueId())) {
            return;
        }

        if (event.getTo() == null) {
            return;
        }

        if (event.getFrom().getBlockX() != event.getTo().getBlockX()
                || event.getFrom().getBlockY() != event.getTo().getBlockY()
                || event.getFrom().getBlockZ() != event.getTo().getBlockZ()) {

            event.setTo(event.getFrom());
        }
    }

    private final Set<String> allowedCommands = Set.of(
        "login",
        "register",
        "helpa"
    );

    @EventHandler(priority = EventPriority.LOW)
    public void onCommand(PlayerCommandPreprocessEvent event) {

        if (plugin.isAuthenticated(
                event.getPlayer().getUniqueId())) {
            return;
        }

        String command = event.getMessage().split(" ")[0]
            .substring(1)
            .toLowerCase();
        
        if (!allowedCommands.contains(command)) {
            event.setCancelled(true);
            event.getPlayer().sendMessage("§cСначала выполните вход!");
        }
    }


    @EventHandler
    public void onQuit(PlayerQuitEvent event) {

        try {
            if (plugin.isAuthenticated(event.getPlayer().getUniqueId())) {
                Location loc = event.getPlayer().getLocation();

                plugin.getDatabase().savePos(
                    loc.getWorld().getName(),
                    loc.getBlockX(),
                    loc.getBlockY(),
                    loc.getBlockZ(),
                    event.getPlayer().getName()
                );
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        plugin.unauthenticate(
            event.getPlayer().getUniqueId()
        );
    }
}
