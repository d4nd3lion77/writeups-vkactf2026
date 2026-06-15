package me.example.auth;

import org.bukkit.Bukkit;
import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;
import org.bukkit.plugin.java.JavaPlugin;
import org.bukkit.Location;
import org.bukkit.GameMode;

import java.util.HashSet;
import java.util.Set;
import java.util.UUID;
import java.util.Random;

public class AuthPlugin extends JavaPlugin {

    private final Set<UUID> authenticated = new HashSet<>();

    public static Random rnd = new Random(12891127129014L);
    public static String first_gen = String.format("%08x", rnd.nextInt());
    public static int gen_counter = 1;

    private DatabaseManager database;
    public DatabaseManager getDatabase() {
        return database;
    }

    @Override
    public void onEnable() {
        Bukkit.getPluginManager().registerEvents(
                new AuthListener(this),
                this
        );

        if (!getDataFolder().exists()) {
            getDataFolder().mkdirs();
        }

    database = new DatabaseManager();

        try {
            database.connect(
                getDataFolder() + "/users.db"
            );
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean isAuthenticated(UUID uuid) {
        return authenticated.contains(uuid);
    }

    public void authenticate(UUID uuid) {
        authenticated.add(uuid);
    }

    public void unauthenticate(UUID uuid) {
        authenticated.remove(uuid);
    }

    @Override
    public boolean onCommand(CommandSender sender,
                             Command command,
                             String label,
                             String[] args) {

        if (!(sender instanceof Player player)) {
            return true;
        }

        if (command.getName().equalsIgnoreCase("helpa")) {

            if (args.length == 1 && args[0].equalsIgnoreCase("about")) {
                player.sendMessage("§aPowered by Java and MySQL\nMy Telegram channel: https://t.me/one1auth \nSpecial Thanks to rayofpower!");
                return true;
            }

            player.sendMessage(
                "§eOne1Auth 1.0\n/login <password>\n/register <password> <password>\n/helpa [about]\n/hexgenerate\n/vip <secret>\n/coupon <code>"
            );

            return true;
        }

        if (command.getName().equalsIgnoreCase("login")) {
            if (this.isAuthenticated(
                    player.getUniqueId())) {
                player.sendMessage(
                    "§cВы уже вошли!"
                );
                return true;
            }
            if (args.length != 1) {
                player.sendMessage(
                    "§eИспользование: /login <password>"
                );
                return true;
            }

            try {

                String username = player.getName();

                if (!database.isRegistered(username)) {

                    player.sendMessage(
                        "§cСначала зарегистрируйтесь"
                    );

                    return true;
                }

                if (!database.checkPassword(
                        username,
                        args[0])) {

                    player.sendMessage(
                        "§cНеверный пароль"
                    );

                    return true;
                }

                authenticate(player.getUniqueId());

                player.sendMessage(
                    "§aВы успешно авторизовались!"
                );

                Location saved =
                    database.getSavedLocation(player);

                if (saved != null) {
                    player.teleport(saved);
                } else {
                    player.teleport(
                        new Location(
                            player.getWorld(),
                            -175,
                            74,
                            600
                        )
                    );
                }

                player.setGameMode(
                    GameMode.SURVIVAL
                );

            } catch (Exception e) {
                e.printStackTrace();
            }

            return true;
        }

        if (command.getName().equalsIgnoreCase("register")) {

            if (args.length != 2) {
                player.sendMessage(
                    "§eПрименение: /register <password> <password>"
                );
                return true;
            }
            if (!args[0].equals(args[1])) {
                player.sendMessage(
                    "§cПароли не совпадают"
                );
                return true;
            }
            if (args[0].length() < 8) {
                player.sendMessage(
                    "§cМинимальная длина пароля: 8 символов"
                );
                return true;
            }

            try {

                String username = player.getName();

                if (database.isRegistered(username)) {
                    player.sendMessage(
                        "§cВы уже зарегистрированы"
                    );
                    return true;
                }

                database.register(username, args[0], player.getWorld().getName(), -175, 74, 600);

                player.sendMessage(
                    "§aРегистрация завершена. Выполните /login"
                );

            } catch (Exception e) {
                e.printStackTrace();
            }

            return true;
        }

        if (command.getName().equalsIgnoreCase("hexgenerate")) {
            player.sendMessage(
                "§a" + String.format("%08x", rnd.nextInt())
            );
            gen_counter++;
            player.sendMessage(
                "§aКоличество успешных генераций:" + gen_counter
            );
            return true;
        }

        if (command.getName().equalsIgnoreCase("vip")) {
            String username = player.getName();
            try {
                if (!database.isRegistered(username)) {
                    player.sendMessage(
                        "§cСначала зарегистрируйтесь"
                    );
                    return true;
                }
                if (database.isVip(username)) {
                    player.sendMessage(
                        "§aВы VIP-игрок!\nВам доступны купоны -> /coupon <code>"
                    );
                    return true;
                }
                if (args.length != 1) {
                    if (!database.isVip(username)) {
                        player.sendMessage(
                            "§cВы не VIP-игрок!\nЧтобы стать им введите /vip <secret>"
                        );
                    }
                    return true;
                }
                if (!args[0].equals(first_gen)) {
                    player.sendMessage(
                        "§cНЕТ УЖ! Это не тот секрет!"
                    );
                    return true;
                }
                database.becomeVip(username);
                player.sendMessage(
                    "§aТеперь вы VIP-игрок!\nВам доступны купоны -> /coupon <code>"
                );
            } catch (Exception e) {
                e.printStackTrace();
            }
            return true;
        }
        
        if (command.getName().equalsIgnoreCase("coupon")) {
            String username = player.getName();
            if (args.length != 1) {
                player.sendMessage(
                    "§cВведите один купон!"
                );
                return true;
            }
            try {
                if (database.isVip(username)) {
                    String code = args[0];
                    player.sendMessage(
                        database.couponCheck(code)
                    );
                } else {
                    player.sendMessage(
                        "§cВы не VIP-игрок!\nЧтобы стать им введите /vip <secret>"
                    );
                }
            } catch (Exception e) {
                e.printStackTrace();
                player.sendMessage(
                    "§cКакая-то ошибка в SQLite..."
                );
            }
            return true;
        }

        return false;
    }
}
