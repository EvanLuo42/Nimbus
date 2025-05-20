import Cocoa
import EventCallbacks

public class Program {
    private static var shouldClose = false
    private static var shiftState = false
    private static var commandState = false
    private static var optionState = false
    private static var controlState = false
    private static var capsLockState = false
    private static var leftMouseState = false
    private static var rightMouseState = false
    private static var middleMouseState = false

    private static var pressedKeys = Set<UInt16>()

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
                pressedKeys.insert(event.keyCode)
            }
            return nil
        }

        NSEvent.addLocalMonitorForEvents(matching: .keyUp) { event in
            PublishKeyUpEvent(event.keyCode)
            pressedKeys.remove(event.keyCode)
            return nil
        }

        NSEvent.addLocalMonitorForEvents(matching: .leftMouseDown) { event in
            PublishLeftClickDown();
            leftMouseState = true;
            return event
        }

        NSEvent.addLocalMonitorForEvents(matching: .leftMouseUp) { event in
            PublishLeftClickUp();
            leftMouseState = false;
            return event
        }

        NSEvent.addLocalMonitorForEvents(matching: .rightMouseDown) { event in
            PublishRightClickDown();
            rightMouseState = true;
            return event
        }

        NSEvent.addLocalMonitorForEvents(matching: .rightMouseUp) { event in
            PublishRightClickUp();
            rightMouseState = false;
            return event
        }

        NSEvent.addLocalMonitorForEvents(matching: .otherMouseDown) { event in
            PublishMiddleClickDown();
            middleMouseState = true;
            return event
        }

        NSEvent.addLocalMonitorForEvents(matching: .otherMouseUp) { event in
            PublishMiddleClickUp();
            middleMouseState = false;
            return event
        }

        NSEvent.addLocalMonitorForEvents(matching: .flagsChanged) { event in
            let newFlags = event.modifierFlags.intersection(.deviceIndependentFlagsMask)
            if newFlags.contains(.shift), !shiftState {
                shiftState = !shiftState
                PublishKeyDownEvent(event.keyCode)
                pressedKeys.insert(event.keyCode)
            } else if !newFlags.contains(.shift), shiftState {
                shiftState = !shiftState
                PublishKeyUpEvent(event.keyCode)
                pressedKeys.remove(event.keyCode)
            }

            if newFlags.contains(.command), !commandState {
                commandState = !commandState
                PublishKeyDownEvent(event.keyCode)
                pressedKeys.insert(event.keyCode)
            } else if !newFlags.contains(.command), commandState {
                commandState = !commandState
                PublishKeyUpEvent(event.keyCode)
                pressedKeys.remove(event.keyCode)
            }

            if newFlags.contains(.option), !optionState {
                optionState = !optionState
                PublishKeyDownEvent(event.keyCode)
                pressedKeys.insert(event.keyCode)
            } else if !newFlags.contains(.option), optionState {
                optionState = !optionState
                PublishKeyUpEvent(event.keyCode)
                pressedKeys.remove(event.keyCode)
            }

            if newFlags.contains(.control), !controlState {
                controlState = !controlState
                PublishKeyDownEvent(event.keyCode)
                pressedKeys.insert(event.keyCode)
            } else if !newFlags.contains(.control), controlState {
                controlState = !controlState
                PublishKeyUpEvent(event.keyCode)
                pressedKeys.remove(event.keyCode)
            }

            if newFlags.contains(.capsLock), !capsLockState {
                capsLockState = !capsLockState
                PublishKeyDownEvent(event.keyCode)
                pressedKeys.insert(event.keyCode)
            } else if !newFlags.contains(.capsLock), capsLockState {
                capsLockState = !capsLockState
                PublishKeyUpEvent(event.keyCode)
                pressedKeys.remove(event.keyCode)
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

    public static func getMousePosition() -> [Double] {
        var position: [Double] = []
        let mouseLocation = NSEvent.mouseLocation
        position.append(mouseLocation.x)
        position.append(mouseLocation.y)
        return position
    }

    public static func isKeyPressed(_ keyCode: UInt16) -> Bool {
        return pressedKeys.contains(keyCode)
    }

    public static func isLeftClicked() -> Bool {
        return leftMouseState
    }

    public static func isRightClicked() -> Bool {
        return rightMouseState
    }

    public static func isMiddleClicked() -> Bool {
        return middleMouseState
    }
}
