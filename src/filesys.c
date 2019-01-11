#include<linux/module.h>
#include<linux/fs.h>
#include<linux/pagemap.h>
#include<linux/mount.h>
#include<linux/init.h>
#include<linux/namei.h>

static int aufs_fill_super(struct super_block *sb, void *data, int silent)
{
        printk("create super_block\n");
        struct inode *inode = NULL;
        struct dentry *root;
        int err;
        sb->s_maxbytes          = MAX_LFS_FILESIZE;
        sb->s_blocksize         = PAGE_CACHE_SIZE;
        sb->s_blocksize_bits    = PAGE_CACHE_SHIFT;
        sb->s_magic             = 0x73757373;
        inode = new_inode(sb);
        if (!inode) { 
                err = -ENOMEM;
                goto fail;
        }
        inode->i_mode = 0755;
        inode->i_uid = current_fsuid();
        inode->i_gid = current_fsgid(); 
        inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
        inode->i_mode |= S_IFDIR;
        inode->i_fop = &simple_dir_operations;
        inode->i_op = &simple_dir_inode_operations;
        // inc reference count for ".".
        inc_nlink(inode);
        root = d_make_root(inode);
        sb->s_root = root;
        if (!root) {
                err = -ENOMEM;
                goto fail;
        }
        return 0;
fail:
        printk("error: new super_block failed!");
        return err;
}







static struct super_block * aufs_mount(struct file_system_type *fs_type,
                                       int flags, const char *dev_name,
                                       void *data)
{
    printk("Enter mount\n");
    return mount_nodev(fs_type,flags,data,aufs_fill_super);
}





static struct file_system_type au_fs_type = {
	.owner   = THIS_MODULE,
	.name    = "comicfs",
    .mount   = aufs_mount,
    .kill_sb = kill_litter_super,
};

static int __init aufs_init(void)
{
    printk("init success!\n");
	int retval;
	retval = register_filesystem(&au_fs_type);
	return retval;
}

static void __exit aufs_exit(void)
{
	unregister_filesystem(&au_fs_type);
}
module_init(aufs_init);
module_exit(aufs_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a simper file sys");
MODULE_VERSION("Ver 0.1");





