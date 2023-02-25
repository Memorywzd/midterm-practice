const roles = {
    admin: {
      permissions: ['edit', 'delete', 'create'],
    },
    editor: {
      permissions: ['edit', 'create'],
    },
    viewer: {
      permissions: ['view'],
    },
  };
  
const permissions = {
    edit: {
      name: 'Edit',
      description: 'Can edit content',
    },
    delete: {
      name: 'Delete',
      description: 'Can delete content',
    },
    create: {
      name: 'Create',
      description: 'Can create content',
    },
    view: {
      name: 'View',
      description: 'Can view content',
    },
  };