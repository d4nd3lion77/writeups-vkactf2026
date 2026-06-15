package me.example.auth;

import java.sql.*;
import org.mindrot.jbcrypt.BCrypt;
import org.bukkit.Bukkit;
import org.bukkit.Location;
import org.bukkit.World;
import org.bukkit.entity.Player;

public class DatabaseManager {

    private Connection connection;

    public void connect(String dbPath) throws SQLException {
        connection = DriverManager.getConnection(
                "jdbc:sqlite:" + dbPath
        );

        try (Statement stmt = connection.createStatement()) {
            stmt.execute("""
                CREATE TABLE IF NOT EXISTS users (
                    username TEXT PRIMARY KEY,
                    password TEXT NOT NULL,
                    world TEXT,
                    posx INTEGER,
                    posy INTEGER,
                    posz INTEGER,
                    vip BOOL DEFAULT FALSE
                )
            """);

            stmt.execute("""
                CREATE TABLE IF NOT EXISTS coupons (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    code TEXT UNIQUE,
                    description TEXT
                )
            """);

            stmt.execute("""
                CREATE TABLE IF NOT EXISTS secret (
                    name TEXT PRIMARY KEY,
                    flag TEXT
                )
            """);

            stmt.execute("""
                INSERT INTO secret (name, flag)
                SELECT 'vka{M1n3cr4Ft_j4vA_rnd_SqlI}', 'vka{M1n3cr4Ft_j4vA_rnd_SqlI}'
                WHERE NOT EXISTS (SELECT 1 FROM secret)
            """);
        }
    }

    public boolean isRegistered(String username) throws SQLException {

        PreparedStatement ps = connection.prepareStatement(
                "SELECT username FROM users WHERE username=?"
        );

        ps.setString(1, username);

        return ps.executeQuery().next();
    }

    public boolean isVip(String username) throws SQLException {

        PreparedStatement ps = connection.prepareStatement(
                "SELECT username FROM users WHERE username=? AND vip=TRUE"
        );

        ps.setString(1, username);

        return ps.executeQuery().next();
    }

    public void becomeVip(String username) throws SQLException {
        try (PreparedStatement ps = connection.prepareStatement(
                "UPDATE users SET vip=TRUE WHERE username=?"
        )) {
            ps.setString(1, username);
            ps.executeUpdate();
        }
    }

    public String couponCheck(String coupon) throws SQLException {

        String query = "SELECT code FROM coupons WHERE code='" + coupon + "'";

        try (Statement statm = connection.createStatement();
            ResultSet rs = statm.executeQuery(query)) {

            if (rs.next()) {
                String res = "§aВаш купон: " + rs.getString("code");
                return res;
            }

            return "§cНу такого купона нет";
        }
    }

    public void register(String username, String password, String worldName, Integer posx, Integer posy, Integer posz)
            throws SQLException {

        String hash = BCrypt.hashpw(
                password,
                BCrypt.gensalt(12)
        );

        PreparedStatement ps = connection.prepareStatement(
                "INSERT INTO users(username,password,world,posx,posy,posz) VALUES(?,?,?,?,?,?)"
        );

        ps.setString(1, username);
        ps.setString(2, hash);
        ps.setString(3, worldName);
        ps.setInt(4, posx);
        ps.setInt(5, posy);
        ps.setInt(6, posz);

        ps.executeUpdate();
    }

    public void savePos(String world, Integer posx, Integer posy, Integer posz, String username)
            throws SQLException {

        PreparedStatement ps = connection.prepareStatement(
                "UPDATE users SET world=?, posx=?, posy=?, posz=? WHERE username=?"
        );

        ps.setString(1, world);
        ps.setInt(2, posx);
        ps.setInt(3, posy);
        ps.setInt(4, posz);
        ps.setString(5, username);

        ps.executeUpdate();
    }

    public Location getSavedLocation(Player player)
            throws SQLException {

        PreparedStatement ps = connection.prepareStatement(
            """
            SELECT world,posx,posy,posz
            FROM users
            WHERE username=?
            """
        );

        ps.setString(1, player.getName());

        ResultSet rs = ps.executeQuery();

        if (!rs.next()) {
            return null;
        }

        String worldName = rs.getString("world");

        World world = Bukkit.getWorld(worldName);

        if (world == null) {
            return null;
        }

        return new Location(
            world,
            rs.getInt("posx"),
            rs.getInt("posy"),
            rs.getInt("posz")
        );
    }

    public boolean checkPassword(String username,
                                 String password)
            throws SQLException {

        PreparedStatement ps = connection.prepareStatement(
                "SELECT password FROM users WHERE username=?"
        );

        ps.setString(1, username);

        ResultSet rs = ps.executeQuery();

        if (!rs.next()) {
            return false;
        }

        String hash = rs.getString("password");

        return BCrypt.checkpw(
                password,
                hash
        );
    }
}
