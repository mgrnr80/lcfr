package lcfr;

public class EcCipher
{
    private long cpp_this;
    
    public EcCipher(
        java.lang.String curve)
        throws java.lang.Exception
    {
        init(
            curve);
    }
    
    public native void init(
        java.lang.String curve)
        throws java.lang.Exception;
    
    public native int getPrimeBitLength()
        throws java.lang.Exception;
    
    public native int getPrimeByteLength()
        throws java.lang.Exception;
    
    public native void getPrime(
        byte[] p)
        throws java.lang.Exception;
    
    public native int getCurvePointCoordinateBitLength()
        throws java.lang.Exception;
    
    public native int getCurvePointCoordinateByteLength()
        throws java.lang.Exception;
    
    public native void generatePublicKey(
        byte[] qx,
        byte[] qy,
        byte[] sk)
        throws java.lang.Exception;
    
    public native void generateSignature(
        byte[] r,
        byte[] s,
        byte[] hash,
        byte[] ek,
        byte[] sk)
        throws java.lang.Exception;
    
    public native int verifySignature(
        byte[] r,
        byte[] s,
        byte[] hash,
        byte[] qx,
        byte[] qy)
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

