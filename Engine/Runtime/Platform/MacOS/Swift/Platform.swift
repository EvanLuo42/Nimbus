import Cocoa
import EventCallbacks

public class Program {
    private static var shouldClose = false
    private static var shiftState = false
    private static var commandState = false
    private static var optionState = false
    private static var controlState = false
    private static var capsLockState = false

    public static func createWindow(_ width: Int32, _ height: Int32, _ title: String) -> UnsafeMutableRawPointer? {
        let app = NSApplication.shared
        app.setActivationPolicy(.regular)

        let window = NSWindow(
            contentRect: NSMakeRect(0, 0, CGFloat(width), CGFloat(height)),
            styleMask: [.titled, .closable, .resizable],
            backing: .buffered,
            defer: false
        )
        window.title = title
        window.makeKeyAndOrderFront(nil)
        app.activate(ignoringOtherApps: true)

        NotificationCenter.default.addObserver(forName: NSWindow.willCloseNotification, object: window, queue: nil) { _ in
            shouldClose = true
        }

        NotificationCenter.default.addObserver(forName: NSWindow.didEndLiveResizeNotification, object: window, queue: nil) { _ in
            PublishResizeEvent(window.frame.size.width, window.frame.size.height)
        }

        NSEvent.addLocalMonitorForEvents(matching: .keyDown) { event in
            if !event.isARepeat {
                PublishKeyDownEvent(event.keyCode)
            }
            return nil
        }

        NSEvent.addLocalMonitorForEvents(matching: .keyUp) { event in
            PublishKeyUpEvent(event.keyCode)
            return nil
        }

        NSEvent.addLocalMonitorForEvents(matching: .flagsChanged) { event in
            let newFlags = event.modifierFlags.intersection(.deviceIndependentFlagsMask)
            if newFlags.contains(.shift), !shiftState {
                shiftState = !shiftState
                PublishKeyDownEvent(event.keyCode)
            } else if !newFlags.contains(.shift), shiftState {
                shiftState = !shiftState
                PublishKeyUpEvent(event.keyCode)
            }

            if newFlags.contains(.command), !commandState {
                commandState = !commandState
                PublishKeyDownEvent(event.keyCode)
            } else if !newFlags.contains(.command), commandState {
                commandState = !commandState
                PublishKeyUpEvent(event.keyCode)
            }

            if newFlags.contains(.option), !optionState {
                optionState = !optionState
                PublishKeyDownEvent(event.keyCode)
            } else if !newFlags.contains(.option), optionState {
                optionState = !optionState
                PublishKeyUpEvent(event.keyCode)
            }

            if newFlags.contains(.control), !controlState {
                controlState = !controlState
                PublishKeyDownEvent(event.keyCode)
            } else if !newFlags.contains(.control), controlState {
                controlState = !controlState
                PublishKeyUpEvent(event.keyCode)
            }

            if newFlags.contains(.capsLock), !capsLockState {
                capsLockState = !capsLockState
                PublishKeyDownEvent(event.keyCode)
            } else if !newFlags.contains(.capsLock), capsLockState {
                capsLockState = !capsLockState
                PublishKeyUpEvent(event.keyCode)
            }

            return nil
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

    public static func setTitle(_ title: String) {
        NSApp.windows.first!.title = title
    }
}
