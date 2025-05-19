import Cocoa

public class Program {
    private static var shouldClose = false

    public static func createWindow(_ width: Int32, _ height: Int32, _ title: String) -> UnsafeMutableRawPointer? {
        let app = NSApplication.shared
        app.setActivationPolicy(.regular)

        let window = NSWindow(
            contentRect: NSMakeRect(0, 0, CGFloat(width), CGFloat(height)),
            styleMask: [.titled, .closable],
            backing: .buffered,
            defer: false
        )
        window.title = title
        window.makeKeyAndOrderFront(nil)
        app.activate(ignoringOtherApps: true)

        NotificationCenter.default.addObserver(forName: NSWindow.willCloseNotification, object: window, queue: nil) { _ in
            shouldClose = true
        }

        return UnsafeMutableRawPointer(Unmanaged.passUnretained(window.contentView!).toOpaque())
    }

    public static func shouldCloseFlag() -> Bool {
        return shouldClose
    }

    public static func pollEvents() {
        let app = NSApplication.shared
        while let event = app.nextEvent(
            matching: .any,
            until: Date(timeIntervalSinceNow: 0),
            inMode: .default,
            dequeue: true
        ) {
            app.sendEvent(event)
            app.updateWindows()
        }
    }
}
