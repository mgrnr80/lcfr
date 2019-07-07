package lcfr;

public class LibraryInfo
{
    private long cpp_this;
    
    public LibraryInfo()
        throws java.lang.Exception
    {
        init();
    }
    
    public native void init()
        throws java.lang.Exception;
    
    public native java.lang.String getVersion()
        throws java.lang.Exception;
    
    public native void destroy()
        throws java.lang.Exception;
    
    public void dispose()
    {
        try { destroy(); } catch (java.lang.Exception ex) {}
    }
    
    protected void finalize()
    {
        dispose();
    }
    
}

